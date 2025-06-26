#include <vector>
#include <memory>
#include <iostream>
#include <cmath>

class TypeAgnosticCallable {
public:
    struct Base {
        virtual int8_t* call() = 0;
    };

    template<typename Functor, typename... Args>
    struct FunctionWrapper : public Base {
        FunctionWrapper(const Functor& func, Args&&... args) : f(func), arguments(std::forward<Args>(args)...) {}


        int8_t* call() override {
            auto tmp = callFunc(std::index_sequence_for<Args...>());
            return reinterpret_cast<int8_t*&>(tmp);
        }

        template<std::size_t... Indices>
        auto callFunc(std::index_sequence<Indices...>) {
            return f(std::get<Indices>(arguments)...);
        }

        Functor f;
        std::tuple<Args...> arguments;
    };

    template<typename Functor, typename... Args>
    TypeAgnosticCallable(const Functor& func, Args&&... args) {
        ptr_ = std::make_shared<FunctionWrapper<Functor, Args...>>(FunctionWrapper<Functor, Args...>(func, std::forward<Args>(args)...));
    }

    TypeAgnosticCallable(const TypeAgnosticCallable& other) {
        ptr_ = other.ptr_;
    }

    int8_t* operator()() const {
        return ptr_->call();
    }

private:
    std::shared_ptr<Base> ptr_;
};


template <typename T>
class FutureResult {
public:
    FutureResult(const TypeAgnosticCallable& func_, size_t id, std::vector<int8_t*>& results) : func(func_), id_(id), results_(results) {};
    operator T() const {
        if (results_[id_] == nullptr) {
            results_[id_] = func();
        }
        return *reinterpret_cast<const T*>(&results_[id_]);
    }
private:
    TypeAgnosticCallable func;
    std::vector<int8_t*> mutable results_;
    size_t id_;
};


class TTaskScheduler {
public:
    template<typename Function, typename... Args>
    size_t add(const Function& func, Args&&... args) {
        results.resize(size + 1, nullptr);
        TypeAgnosticCallable tmp(func, std::forward<Args>(args)...);
        container.push_back(tmp);
        return size++;
    }

    template<typename T>
    T getResult(size_t id) {
        if (results[id] == nullptr) {
            results[id] = container[id]();
        }
        return *reinterpret_cast<const T*>(&results[id]);
    }

    template<typename T>
    FutureResult<T> getFutureResult(size_t id) {
        return FutureResult<T>(container[id], id, results);
    }

    void executeAll() {
        for (int i = 0; i < container.size(); ++i) {
            if (results[i] == nullptr) {
                results[i] = container[i]();
            }
        }
    }
private:
    std::vector<TypeAgnosticCallable> container;
    std::vector<int8_t*> results;
    size_t size = 0;
};