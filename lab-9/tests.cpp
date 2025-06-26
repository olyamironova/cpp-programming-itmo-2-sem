#include <gtest/gtest.h>
#include "adapters.cpp"
#include <map>
#include <unordered_map>
#include <vector>
#include <list>

TEST(DropAdapterTest, DropElementsFromVector) {
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };

    drop::drop_helper helper(2);
    auto result = numbers | drop::drop(helper);

    std::vector<int> expected = { 3, 4, 5 };
    ASSERT_EQ(std::vector<int>(result.begin(), result.end()), expected);
}

TEST(DropAdapterTest, DropElementsFromList) {
    std::list<char> characters = { 'a', 'b', 'c', 'd', 'e' };

    drop::drop_helper helper(3);
    auto result = characters | drop::drop(helper);

    std::list<char> expected = { 'd', 'e' };
    ASSERT_EQ(std::list<char>(result.begin(), result.end()), expected);
}

TEST(DropAdapterTest, DropZeroElements) {
    std::array<float, 4> floats = { 1.1, 2.2, 3.3, 4.4 };

    drop::drop_helper helper(0);
    auto result = floats | drop::drop(helper);

    std::array<float, 4> expected = { 1.1, 2.2, 3.3, 4.4 };
    ASSERT_EQ(std::array<float, 4>(result.begin(), result.end()), expected);
}

TEST(DropAdapterTest, DropTooManyElements) {
    std::deque<int> deque = { 1, 2, 3 };

    drop::drop_helper helper(5);
    auto result = deque | drop::drop(helper);

    ASSERT_EQ(result.begin(), result.end());
}





TEST(TakeAdapterTest, TakeFirstElementsFromVector) {
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };

    take::take_helper helper(3);
    auto result = numbers | take::take(helper);

    std::vector<int> expected = { 1, 2, 3 };
    ASSERT_EQ(std::vector<int>(result.begin(), result.end()), expected);
}

TEST(TakeAdapterTest, TakeFirstElementsFromList) {
    std::list<char> characters = { 'a', 'b', 'c', 'd', 'e' };

    take::take_helper helper(2);
    auto result = characters | take::take(helper);

    std::list<char> expected = { 'a', 'b' };
    ASSERT_EQ(std::list<char>(result.begin(), result.end()), expected);
}

TEST(TakeAdapterTest, TakeAllElementsFromDeque) {
    std::deque<float> floats = { 1.1, 2.2, 3.3 };

    take::take_helper helper(3);
    auto result = floats | take::take(helper);

    std::deque<float> expected = { 1.1, 2.2, 3.3 };
    ASSERT_EQ(std::deque<float>(result.begin(), result.end()), expected);
}

TEST(TakeAdapterTest, TakeMoreElementsThanExist) {
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };

    take::take_helper helper(10);
    auto result = numbers | take::take(helper);

    std::vector<int> expected = { 1, 2, 3, 4, 5 };
    ASSERT_EQ(std::vector<int>(result.begin(), result.end()), expected);
}

TEST(TakeAdapterTest, TakeZeroElements) {
    std::list<std::string> strings = { "hello", "world" };

    take::take_helper helper(0);
    auto result = strings | take::take(helper);

    ASSERT_EQ(result.begin(), result.end());
}

TEST(TakeAdapterTest, TakeVectorWithLambda) {
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };

    auto result = numbers | take::take(3);

    std::vector<int> expected = { 1, 2, 3 };
    ASSERT_EQ(std::vector<int>(result.begin(), result.end()), expected);
}

TEST(TakeAdapterTest, TakeListWithLambda) {
    std::list<float> floats = { 1.1, 2.2, 3.3, 4.4, 5.5 };

    auto result = floats | take::take(2);

    std::list<float> expected = { 1.1, 2.2 };
    ASSERT_EQ(std::list<float>(result.begin(), result.end()), expected);
}





// A simple function to double an integer
int Double(int x) {
    return x * 2;
}

// A function object to square a float
struct Square {
    float operator()(float x) const {
        return x * x;
    }
};

TEST(TransformAdapterTest, TransformVectorWithFunction) {
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };

    auto result = numbers | transform::transform(Double);

    std::vector<int> expected = { 2, 4, 6, 8, 10 };
    ASSERT_EQ(std::vector<int>(result.begin(), result.end()), expected);
}

TEST(TransformAdapterTest, TransformListWithFunctionObject) {
    std::list<float> floats = { 1.1, 2.2, 3.3, 4.4, 5.5 };

    Square square;
    auto result = floats | transform::transform(square);

    std::list<float> expected = { 1.21, 4.84, 10.89, 19.36, 30.25 };
    ASSERT_EQ(std::list<float>(result.begin(), result.end()), expected);
}

TEST(TransformAdapterTest, TransformVectorWithLambda) {
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };

    auto result = numbers | transform::transform([](int x) { return x * 3; });

    std::vector<int> expected = { 3, 6, 9, 12, 15 };
    ASSERT_EQ(std::vector<int>(result.begin(), result.end()), expected);
}

TEST(TransformAdapterTest, TransformListWithLambda) {
    std::list<float> floats = { 1.1, 2.2, 3.3, 4.4, 5.5 };

    auto result = floats | transform::transform([](float x) { return x + 1.0; });

    std::list<float> expected = { 2.1, 3.2, 4.3, 5.4, 6.5 };
    ASSERT_EQ(std::list<float>(result.begin(), result.end()), expected);
}






TEST(KeysAdapterTest, KeysFromMap) {
    std::map<int, std::string> myMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = myMap | keys::keys({ 1, 3 });

    std::vector<std::pair<int, std::string>> expected = {
        {2, "two"}
    };

    ASSERT_EQ(std::vector<std::pair<int, std::string>>(result.begin(), result.end()), expected);
}

TEST(KeysAdapterTest, KeysFromUnorderedMap) {
    std::unordered_map<int, std::string> myUnorderedMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = myUnorderedMap | keys::keys({ 1, 3 });

    std::vector<std::pair<int, std::string>> expected = {
        {2, "two"}
    };

    ASSERT_EQ(std::vector<std::pair<int, std::string>>(result.begin(), result.end()), expected);
}

TEST(KeysAdapterTest, KeysWithLambdaMap) {
    std::map<int, std::string> myMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = myMap | keys::keys([](const auto& pair) { return pair.first % 2 == 0; });

    std::vector<std::pair<int, std::string>> expected = {
        {1, "one"},
        {3, "three"}
    };

    ASSERT_EQ(std::vector<std::pair<int, std::string>>(result.begin(), result.end()), expected);
}

TEST(KeysAdapterTest, KeysWithLambdaUnorderedMap) {
    std::unordered_map<int, std::string> myUnorderedMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = myUnorderedMap | keys::keys([](const auto& pair) { return pair.first % 2 == 0; });

    std::vector<std::pair<int, std::string>> expected = {
        {1, "one"},
        {3, "three"}
    };

    ASSERT_EQ(std::vector<std::pair<int, std::string>>(result.begin(), result.end()), expected);
}



TEST(ValuesAdapterTest, ValuesWithLambdaMap) {
    std::map<int, std::string> myMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = myMap | values::values([](const auto& pair) { return pair.second.length() > 3; });

    std::vector<std::pair<int, std::string>> expected = {
        {2, "two"},
        {3, "three"}
    };

    ASSERT_EQ(std::vector<std::pair<int, std::string>>(result.begin(), result.end()), expected);
}

TEST(ValuesAdapterTest, ValuesWithLambdaUnorderedMap) {
    std::unordered_map<int, std::string> myUnorderedMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = myUnorderedMap | values::values([](const auto& pair) { return pair.second.length() > 3; });

    std::vector<std::pair<int, std::string>> expected = {
        {2, "two"},
        {3, "three"}
    };

    ASSERT_EQ(std::vector<std::pair<int, std::string>>(result.begin(), result.end()), expected);
}

TEST(ValuesAdapterTest, ValuesWithLambdaMap) {
    std::map<int, std::string> myMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = myMap | values::values([](const auto& pair) { return pair.first % 2 == 0; });

    std::vector<std::string> expected = { "two" };

    ASSERT_EQ(std::vector<std::string>(result.begin(), result.end()), expected);
}

TEST(ValuesAdapterTest, ValuesWithLambdaUnorderedMap) {
    std::unordered_map<int, std::string> myUnorderedMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    auto result = myUnorderedMap | values::values([](const auto& pair) { return pair.first % 2 == 0; });

    std::vector<std::string> expected = { "two" };

    ASSERT_EQ(std::vector<std::string>(result.begin(), result.end()), expected);
}




TEST(ReverseAdapterTest, ReverseVector) {
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    auto reversed = vec | reverse::reverse();

    std::vector<int> expected = { 5, 4, 3, 2, 1 };
    ASSERT_EQ(reversed.container, expected);
}

TEST(ReverseAdapterTest, ReverseList) {
    std::list<int> lst = { 1, 2, 3, 4, 5 };
    auto reversed = lst | reverse::reverse();

    std::list<int> expected = { 5, 4, 3, 2, 1 };
    ASSERT_EQ(reversed.container, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




TEST(FilterAdapterTest, FilterVector) {
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    auto filtered = vec | filter([](int x) { return x % 2 == 0; });

    auto expected = { 2, 4 };
    ASSERT_EQ(filtered.container, expected);
}

TEST(FilterAdapterTest, FilterList) {
    std::list<int> lst = { 1, 2, 3, 4, 5 };
    auto filtered = lst | filter([](int x) { return x % 2 == 0; });

    auto expected = { 2, 4 };
    ASSERT_EQ(filtered.container, expected);
}

TEST(FilterAdapterTest, FilterEmptyContainer) {
    std::vector<int> vec;
    auto filtered = vec | filter([](int x) { return x % 2 == 0; });

    ASSERT_TRUE(filtered.container.empty());
}

TEST(FilterAdapterTest, FilterAllElements) {
    std::vector<int> vec = { 2, 4, 6, 8, 10 };
    auto filtered = vec | filter([](int x) { return x % 2 == 0; });

    ASSERT_EQ(filtered.container, vec);
}

TEST(FilterAdapterTest, FilterNoElements) {
    std::vector<int> vec = { 1, 3, 5, 7, 9 };
    auto filtered = vec | filter([](int x) { return x % 2 == 0; });

    ASSERT_TRUE(filtered.container.empty());
}

TEST(FilterAdapterTest, FilterUsingLambdaWithState) {
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    int count = 0;
    auto filtered = vec | filter([&count](int x) {
        if (x % 2 == 0) {
            count++;
            return true;
        }
        return false;
        });

    auto expected = { 2, 4 };
    ASSERT_EQ(filtered.container, expected);
    ASSERT_EQ(count, 2);
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
