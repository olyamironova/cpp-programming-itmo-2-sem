#pragma once
#include <iterator>
#include <stdint.h>
#include <initializer_list>
#include <iostream>
#include <type_traits>

namespace drop {
	struct drop_helper {
		size_t helper;
		drop_helper(size_t d) : helper(d) {}
	};

	template <typename Container>
	struct dropped_range {
		using iterator = typename Range::iterator;
		iterator begin_;
		Container container;

		dropped_range(Container& c, size_t amount) : container(c) {
			begin_ = c.begin();
			for (size_t i = 0; i < amount; ++i) {
				begin_++;
			}
		}

		template<typename Iter> class drop_iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = typename Container::value_type;
			using pointer = value_type*;
			using reference = value_type&;

			drop_iterator this_iter;
			drop_iterator(const drop_iterator& other_iter) : this_iter(other_iter) {}

			drop_iterator& operator++() {
				this_iter++;
				return *this;
			}

			drop_iterator& operator++(int) {
				drop_iterator tmp(*this);
				++begin_;
				return tmp;
			}

			value_type operator*() {
				return *this_iter;
			}

			pointer operator->() const {
				return std::addressof(operator*());
			}

			bool operator==(drop_iterator<Iter>& other) {
				return (this->this_iter == other.this_iter);
			}

			bool operator!=(drop_iterator<Iter>& other) {
				return !(this->this_iter == other.this_iter);
			}
		};

		drop_iterator<iterator> begin() {
			return Iter<iterator>(begin_);
		}

		drop_iterator<iterator> end() {
			return Iter<iterator>(c.end());
		}
	};

	template<typename Container1, typename Container2>
	struct is_compatible {
		static constexpr bool value = std::is_same_v<Container1, Container2>;
	};

	template <typename Container1, typename Container2>
	constexpr bool is_compatible_v = is_compatible<Container1, Container2>::value;

	template <typename Container1, typename Container2>
	using enable_if_compatible = std::enable_if_t<is_compatible_v<Container1, Container2>, int>;

	template <typename Container>
	dropped_range<Container> operator|(Container& c, const drop_helper& f) {
		return dropped_range<Container>(c, f.helper);
	}

	template <typename Container>
	dropped_range<Container>operator|(const Container& c, const drop_helper& f) {
		return dropped_range<Container>(c, f.helper);
	}

	template <typename Container1, typename Container2, typename = enable_if_compatible<Container1, Container2>>
	dropped_range<Container1> operator|(dropped_range<Container1>& c, const drop_helper& f) {
		static_assert(is_compatible_v<Container1, Container2>, "Containers are not compatible");
		return dropped_range<Container1>(c, f.helper);
	}
	
}; // namespace drop

drop::drop_helper drop(size_t x) {
	return drop::drop_helper(x);
}






namespace take {
	struct take_helper {
		size_t helper;
		take_helper(size_t r) : helper(r) {}
	};

	template <typename Container>
	struct take_range {
		using iterator = typename Container::iterator;

		iterator end_;
		Container container;

		take_range(Container& c, size_t amount) : container(c) {
			end_ = c.begin();
			for (size_t i = 0; i < amount; ++i) {
				end_++;
			}
		}

		template<typename Iterator>
		struct take_iterator {
			using iterator_category = std::forward_iterator_tag;
			using value_type = typename Container::value_type;
			using pointer = Iterator*;
			using reference = Iterator&;

			Iterator this_iter;
			take_iterator(const Iterator* other_iter) : this_iter(other_iter) {}

			take_iterator& operator++() {
				this_iter++;
				return *this;
			}

			take_iterator& operator++(int) {
				take_iterator tmp(*this);
				++(*this);
				return tmp;
			}

			value_type operator*() {
				return *this_iter;
			}

			pointer operator->() const {
				return std::addressof(operator*());
			}

			bool operator==(const take_iterator<Iterator>& other) const {
				return (this->this_iter == other.this_iter);
			}

			bool operator!=(const take_iterator<Iterator>& other) const {
				return !(this->this_iter == other.this_iter);
			}
		};

		take_iterator<iterator> begin() {
			return take_iterator<iterator>(container.begin());
		}

		take_iterator<iterator> end() {
			return take_iterator<iterator>(this->end_);
		}
	};

	template <typename Container>
	take_range<Container> operator|(Container& c, const take_helper& h) {
		return take_range<Container>(c, h.helper);
	}

	template <typename Container1, typename Container2>
	take_range<Container1> operator|(take_range<Container1>& c, const take_helper& h) {
		static_assert(std::is_same_v<Container1, Container2>, "Containers are not compatible");
		return take_range<Container1>(c, h.helper);
	}
}; // namespace take

take::take_helper take(size_t h) {
	return take::take_helper(h);
}





namespace transform {
	template <typename Value>
	struct transform_helper {
		Value helper;
		transform_helper(Value v) : helper(v) {}
	};

	template<typename Container, typename Replacement>
	struct transform_range {

		transform_range(Container& c, const Replacement& r) : container(c), condition(r) {}

		using container_iterator = typename Container::iterator;

		template<typename Iterator>
		struct transform_iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = typename Container::value_type;
			using pointer = Iterator*;
			using reference = Iterator&;

			Replacement condition;
			Iterator this_iter;

			transform_iterator(const Replacement& what, const Iterator& other_iter) : condition(what), this_iter(other_iter) {}

			bool operator==(const transform_iterator<Iterator>& other) const {
				return (this->this_iter == other.this_iter);
			}

			bool operator!=(const transform_iterator<Iterator>& other) const {
				return !(this->this_iter == other.this_iter);
			}

			transform_iterator& operator++() {
				this_iter++;
				return *this;
			}

			transform_iterator operator++(int) {
				transform_iterator tmp(*this);
				++(*this);
				return tmp;
			}

			value_type operator*() {
				return condition(*this_iter);
			}
		};

		transform_iterator<container_iterator> begin() {
			return transform_iterator<container_iterator>(condition, container.begin());
		}

		transform_iterator<container_iterator> end() {
			return transform_iterator<container_iterator>(condition, container.end());
		}

		Container container;
		Replacement condition;
	};

	template <typename Container1, typename Replacement>
	transform_range<Container1, Replacement> operator|(transform_range<Container1, Replacement>& c, const transform_helper<Replacement>& h) {
		static_assert(std::is_same_v<Replacement, typename Container1::value_type>, "Container and Replacement types are incompatible");
		return transform_range<Container1, Replacement>(c.container, h.helper);
	}

	template <typename Container, typename Replacement>
	transform_range<Container, Replacement> operator|(Container& c, const transform_helper<Replacement>& h) {
		return transform_range<Container, Replacement>(c, h.helper);
	}

	template <typename Container, typename Replacement>
	transform_range<Container, Replacement> operator|(const Container& c, const transform_helper<Replacement>& h) {
		return transform_range<Container, Replacement>(c, h.helper);
	}
}; // namespace transform

template <typename Replacement>
transform::transform_helper<Replacement> transform(const Replacement& h) {
	return transform::transform_helper<Replacement>(h);
}






namespace keys {
	template <typename Value>
	struct keys_helper {
		std::initializer_list<Value> helper;
		keys_helper(const std::initializer_list<Value>& values) : helper(values) {}
	};

	template<typename Container, typename Value>
	struct keys_distance {
		using container_iterator = typename Container::iterator;
		using key_type = typename Container::key_type;
		using value_type = typename Container::value_type;

		std::initializer_list<Value> data;
		Container container;

		keys_distance(Container& c, const std::initializer_list<Value>& values)
			: container(c), data(values) {}

		template<typename Iterator>
		struct keys_iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using pointer = Iterator*;
			using reference = Iterator&;

			Iterator begin_;
			Iterator end_;
			std::initializer_list<Value> data;

			explicit keys_iterator(const Iterator& a, const Iterator& b, const std::initializer_list<Value>& d)
				: begin_(a), end_(b), data(d) {}

			bool check_keys(const value_type& key_value) {
				for (const key_type& t : data) {
					if (key_value.first == t) {
						return false;
					}
				}
				return true;
			}

			keys_iterator& operator++() {
				++begin_;
				while (begin_ != end_ && check_keys(*begin_)) {
					++begin_;
				}
				return *this;
			}

			keys_iterator operator++(int) {
				keys_iterator tmp(*this);
				++(*this);
				return tmp;
			}

			value_type operator*() {
				if (begin_ != end_ && check_keys(*begin_)) {
					++(*this);
				}
				return *begin_;
			}

			bool operator==(const keys_iterator& other) const {
				return (begin_ == other.begin_);
			}

			bool operator!=(const keys_iterator& other) const {
				return !(*this == other);
			}
		};

		keys_iterator<container_iterator> begin() {
			return keys_iterator<container_iterator>(container.begin(), container.end(), data);
		}

		keys_iterator<container_iterator> end() {
			return keys_iterator<container_iterator>(container.end(), container.end(), data);
		}
	};

	template<typename Container, typename Value>
	keys_distance<Container, Value> operator|(Container& c, const keys_helper<Value>& v) {
		return keys_distance<Container, Value>(c, v.helper);
	}

	template<typename Container, typename Value>
	keys_distance<Container, Value> operator|(const Container& c, const keys_helper<Value>& v) {
		return keys_distance<Container, Value>(c, v.helper);
	}
}; // namespace keys

template<typename Value>
keys::keys_helper<Value> keys(std::initializer_list<Value> v) {
	return keys::keys_helper<Value>(v);
}








namespace values {
	template<typename Value>
	struct values_helper {
		std::initializer_list<Value> helper;
		values_helper(const std::initializer_list<Value>& values)
			: helper(values) {}
	};

	template<typename Container, typename Value>
	struct values_distance {
		using container_iterator = typename Container::iterator;
		using value_type = typename Container::value_type;
		using mapped_type = typename Container::mapped_type;

		std::initializer_list<Value> data;
		Container container;

		values_distance(Container& c, const std::initializer_list<Value>& values)
			: container(c), data(values) {}

		template<typename Iterator>
		struct values_iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using pointer = Iterator*;
			using reference = Iterator&;

			Iterator begin_;
			Iterator end_;
			std::initializer_list<Value> data;

			explicit values_iterator(const Iterator& a, const Iterator& b, const std::initializer_list<Value>& d)
				: begin_(a), end_(b), data(d) {}

			bool check_values(const value_type& value) {
				for (const mapped_type& m : data) {
					if (value.second == m) {
						return true;
					}
				}
				return false;
			}

			values_iterator& operator++() {
				++begin_;
				while (begin_ != end_ && check_values(*begin_)) {
					++begin_;
				}
				return *this;
			}

			values_iterator operator++(int) {
				values_iterator tmp(*this);
				++(*this);
				return tmp;
			}

			value_type operator*() {
				if (begin_ != end_ && check_values(*begin_)) {
					++(*this);
				}
				return *begin_;
			}

			bool operator==(const values_iterator& other) const {
				return (begin_ == other.begin_);
			}

			bool operator!=(const values_iterator& other) const {
				return !(*this == other);
			}
		};

		values_iterator<container_iterator> begin() {
			return values_iterator<container_iterator>(container.begin(), container.end(), data);
		}

		values_iterator<container_iterator> end() {
			return values_iterator<container_iterator>(container.end(), container.end(), data);
		}
	};

	template<typename Container, typename Value>
	values_distance<Container, Value> operator|(Container& c, const values_helper<Value>& values) {
		static_assert(std::is_same_v<Value, typename Container::mapped_type>, "Container and Value types are incompatible");
		return values_distance<Container, Value>(c, values.helper);
	}

	template<typename Container, typename Value>
	values_distance<Container, Value> operator|(const Container& c, const values_helper<Value>& values) {
		static_assert(std::is_same_v<Value, typename Container::mapped_type>, "Container and Value types are incompatible");
		return values_distance<Container, Value>(c, values.helper);
	}
}; // namespace values

template<typename Value>
values::values_helper<Value> values(std::initializer_list<Value> v) {
	return values::values_helper<Value>(v);
}







namespace reverse {
	struct reverse_helper {};

	template<typename Container>
	struct reverse_distance {
		using container_iterator = typename Container::iterator;
		using value_type = typename Container::value_type;

		container_iterator end_;
		Container container;

		reverse_distance(Container& c) : container(c) {}

		template<typename Iterator>
		struct reverse_iterator {
			using difference_type = std::ptrdiff_t;
			using value_type = typename Container::value_type;
			using iterator_category = std::forward_iterator_tag;
			using pointer = Iterator*;
			using reference = Iterator&;

			Iterator temp_;
			reverse_iterator(const Iterator& other)
				: temp_(other.temp_) {}

			reverse_iterator& operator++() {
				--temp_;
				return *this;
			}

			reverse_iterator operator++(int) {
				reverse_iterator tmp(*this);
				--temp_;
				return tmp;
			}

			value_type operator*() {
				return *temp_;
			}

			bool operator==(const reverse_iterator<Iterator>& other) const {
				return (temp_ == other.temp_);
			}

			bool operator!=(const reverse_iterator<Iterator>& other) const {
				return !(temp_ == other.temp_);
			}
		};

		reverse_iterator<container_iterator> begin() {
			return reverse_iterator<container_iterator>(--container.end());
		}

		reverse_iterator<container_iterator> end() {
			return reverse_iterator<container_iterator>(--container.begin());
		}
	};

	template<typename Container1, typename Container2>
	using enable_if_compatible = std::enable_if_t<std::is_same_v<typename Container1::value_type, typename Container2::value_type>, int>;

	template<typename Container1, typename Container2, typename = enable_if_compatible<Container1, Container2>>
	reverse_distance<Container1> operator|(reverse_distance<Container1>& c, const reverse_helper& h) {
		static_assert(std::is_same_v<typename Container1::value_type, typename Container2::value_type>, "Container types are incompatible");
		return reverse_distance<Container1>(c.container);
	}

	template<typename Container1, typename Container2, typename = enable_if_compatible<Container1, Container2>>
	reverse_distance<Container1> operator|(const reverse_distance<Container1>& c, const reverse_helper& h) {
		static_assert(std::is_same_v<typename Container1::value_type, typename Container2::value_type>, "Container types are incompatible");
		return reverse_distance<Container1>(c.container);
	}
}; // namespace reverse

reverse::reverse_helper reverse() {
	return reverse::reverse_helper();
}






namespace filter {
	template <typename Value>
	struct filter_helper {
		Value helper;
		filter_helper(Value v) : helper(v) {}
	};

	template<typename Container, typename Condition>
	struct filter_distance {
		Container container;
		Condition condition;

		filter_distance(Container& c, const Condition& x) : container(c), condition(x) {}

		using container_iterator = typename Container::iterator;

		template<typename Iterator>
		struct filter_iterator {
			using value_type = typename Container::value_type;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			using pointer = Iterator*;
			using reference = Iterator&;

			Condition condition;
			Iterator begin_;
			Iterator end_;

			filter_iterator(const Condition& c, const Iterator& a, const Iterator& b)
				: condition(c), begin_(a), end_(b) {}

			filter_iterator& operator++() {
				if (begin_ != end_) {
					do {
						++begin_;
					} while (begin_ != end_ && !condition(*begin_));
				}
				return *this;
			}

			filter_iterator operator++(int) {
				filter_iterator tmp(*this);
				++(*this);
				return tmp;
			}

			value_type operator*() {
				if (!condition(*begin_)) {
					++(*this);
				}
				return *begin_;
			}

			bool operator==(const filter_iterator& other) const {
				return (begin_ == other.begin_);
			}

			bool operator!=(const filter_iterator& other) const {
				return !(*this == other);
			}
		};

		filter_iterator<container_iterator> begin() {
			return filter_iterator<container_iterator>(condition, container.begin(), container.end());
		}

		filter_iterator<container_iterator> end() {
			return filter_iterator<container_iterator>(condition, container.end(), container.end());
		}
	};

	template<typename Container1, typename Container2>
	using enable_if_compatible = std::enable_if_t<std::is_same_v<typename Container1::value_type, typename Container2::value_type>, int>;

	template<typename Container1, typename Container2, typename = enable_if_compatible<Container1, Container2>>
	filter_distance<Container1, Container2> operator|(filter_distance<Container1, Container2>& c, const filter_helper<Container2>& h) {
		static_assert(std::is_same_v<typename Container1::value_type, typename Container2::value_type>, "Container types are incompatible");
		return filter_distance<Container1, Container2>(c.container, h.helper);
	}

	template<typename Container1, typename Container2, typename = enable_if_compatible<Container1, Container2>>
	filter_distance<Container1, Container2> operator|(const filter_distance<Container1, Container2>& c, const filter_helper<Container2>& h) {
		static_assert(std::is_same_v<typename Container1::value_type, typename Container2::value_type>, "Container types are incompatible");
		return filter_distance<Container1, Container2>(c.container, h.helper);
	}
}; // namespace filter

template<typename Condition>
filter::filter_helper<Condition> filter(const Condition& h) {
	return filter::filter_helper<Condition>(h);
}

