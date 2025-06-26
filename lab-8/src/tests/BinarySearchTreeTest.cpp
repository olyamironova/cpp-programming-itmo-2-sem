#include <gtest/gtest.h>
#include "inorder_set.h"
#include "preorder_set.h"
#include "postorder_set.h"

using namespace inorder;

TEST(SetTest, ConstructorsAndBasicMethods) {
    inorder::set<int> emptySet;
    EXPECT_TRUE(emptySet.empty());
    EXPECT_EQ(emptySet.size(), 0);

    inorder::set<int> set = { 1, 2, 3, 4, 5 };
    EXPECT_FALSE(set.empty());
    EXPECT_EQ(set.size(), 5);

    inorder::set<int> copySet(set);
    EXPECT_EQ(copySet.size(), set.size());

    inorder::set<int> assignedSet;
    assignedSet = set;
    EXPECT_EQ(assignedSet.size(), set.size());
}

TEST(SetTest, InsertAndFind) {
    inorder::set<int> set;
    auto [it, inserted] = set.insert(5);
    EXPECT_TRUE(inserted);
    EXPECT_EQ(*it, 5);
    EXPECT_EQ(set.size(), 1);

    it = set.find(5);
    EXPECT_NE(it, set.end());
    EXPECT_EQ(*it, 5);

    it = set.find(10);
    EXPECT_EQ(it, set.end());
}

TEST(SetTest, Erase) {
    inorder::set<int> set = { 1, 2, 3, 4, 5 };
    auto it = set.find(3);
    set.erase(it);
    EXPECT_EQ(set.size(), 4);

    it = set.find(3);
    EXPECT_EQ(it, set.end());
}

TEST(SetTest, Clear) {
    inorder::set<int> set = { 1, 2, 3, 4, 5 };
    set.clear();
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0);
}

TEST(SetTest, Swap) {
    inorder::set<int> set1 = { 1, 2, 3 };
    inorder::set<int> set2 = { 4, 5, 6 };
    set1.swap(set2);
    EXPECT_EQ(set1.size(), 3);
    EXPECT_EQ(set2.size(), 3);
}

TEST(SetTest, Merge) {
    inorder::set<int> set1 = { 1, 2, 3 };
    inorder::set<int> set2 = { 3, 4, 5 };
    set1.merge(set2);
    EXPECT_EQ(set1.size(), 4);
    EXPECT_EQ(set2.size(), 2);

    auto it = set1.find(4);
    EXPECT_NE(it, set1.end());
    EXPECT_EQ(*it, 4);
}

TEST(InorderSetTest, InsertAndFind) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    EXPECT_TRUE(mySet.contains(5));
    EXPECT_TRUE(mySet.contains(3));
    EXPECT_TRUE(mySet.contains(7));
    EXPECT_FALSE(mySet.contains(10));
}

TEST(InorderSetTest, SizeAndEmpty) {
    set<int> mySet;
    EXPECT_TRUE(mySet.empty());
    EXPECT_EQ(mySet.size(), 0);

    mySet.insert(5);
    EXPECT_FALSE(mySet.empty());
    EXPECT_EQ(mySet.size(), 1);
}

TEST(InorderSetTest, Clear) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    mySet.clear();
    EXPECT_TRUE(mySet.empty());
    EXPECT_EQ(mySet.size(), 0);
}

TEST(InorderSetTest, Iterator) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    std::vector<int> expectedValues = { 3, 5, 7 };
    std::vector<int> actualValues;
    for (auto it = mySet.begin(); it != mySet.end(); ++it) {
        actualValues.push_back(*it);
    }
    EXPECT_EQ(actualValues, expectedValues);
}

TEST(InorderSetTest, CopyConstructorAndAssignment) {
    set<int> originalSet;
    originalSet.insert(5);
    originalSet.insert(3);
    originalSet.insert(7);

    set<int> copiedSet(originalSet);
    EXPECT_EQ(copiedSet.size(), 3);
    EXPECT_TRUE(copiedSet.contains(5));
    EXPECT_TRUE(copiedSet.contains(3));
    EXPECT_TRUE(copiedSet.contains(7));

    set<int> assignedSet;
    assignedSet = originalSet;
    EXPECT_EQ(assignedSet.size(), 3);
    EXPECT_TRUE(assignedSet.contains(5));
    EXPECT_TRUE(assignedSet.contains(3));
    EXPECT_TRUE(assignedSet.contains(7));
}

TEST(InorderSetTest, Merge) {
    set<int> set1;
    set1.insert(5);
    set1.insert(3);
    set1.insert(7);

    set<int> set2;
    set2.insert(2);
    set2.insert(4);
    set2.insert(6);

    set1.merge(set2);
    EXPECT_EQ(set1.size(), 6);
    EXPECT_TRUE(set1.contains(2));
    EXPECT_TRUE(set1.contains(3));
    EXPECT_TRUE(set1.contains(4));
    EXPECT_TRUE(set1.contains(5));
    EXPECT_TRUE(set1.contains(6));
    EXPECT_TRUE(set1.contains(7));
}

using namespace preorder;

TEST(PreorderSetTest, InsertAndFind) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    EXPECT_TRUE(mySet.contains(5));
    EXPECT_TRUE(mySet.contains(3));
    EXPECT_TRUE(mySet.contains(7));
    EXPECT_FALSE(mySet.contains(10));
}

TEST(PreorderSetTest, SizeAndEmpty) {
    set<int> mySet;
    EXPECT_TRUE(mySet.empty());
    EXPECT_EQ(mySet.size(), 0);

    mySet.insert(5);
    EXPECT_FALSE(mySet.empty());
    EXPECT_EQ(mySet.size(), 1);
}

TEST(PreorderSetTest, Clear) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    mySet.clear();
    EXPECT_TRUE(mySet.empty());
    EXPECT_EQ(mySet.size(), 0);
}

TEST(PreorderSetTest, Iterator) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    std::vector<int> expectedValues = { 5, 3, 7 };
    std::vector<int> actualValues;
    for (auto it = mySet.begin(); it != mySet.end(); ++it) {
        actualValues.push_back(*it);
    }
    EXPECT_EQ(actualValues, expectedValues);
}

TEST(PreorderSetTest, CopyConstructorAndAssignment) {
    set<int> originalSet;
    originalSet.insert(5);
    originalSet.insert(3);
    originalSet.insert(7);

    set<int> copiedSet(originalSet);
    EXPECT_EQ(copiedSet.size(), 3);
    EXPECT_TRUE(copiedSet.contains(5));
    EXPECT_TRUE(copiedSet.contains(3));
    EXPECT_TRUE(copiedSet.contains(7));

    set<int> assignedSet;
    assignedSet = originalSet;
    EXPECT_EQ(assignedSet.size(), 3);
    EXPECT_TRUE(assignedSet.contains(5));
    EXPECT_TRUE(assignedSet.contains(3));
    EXPECT_TRUE(assignedSet.contains(7));
}

TEST(PreorderSetTest, Merge) {
    set<int> set1;
    set1.insert(5);
    set1.insert(3);
    set1.insert(7);

    set<int> set2;
    set2.insert(2);
    set2.insert(4);
    set2.insert(6);

    set1.merge(set2);
    EXPECT_EQ(set1.size(), 6);
    EXPECT_TRUE(set1.contains(2));
    EXPECT_TRUE(set1.contains(3));
    EXPECT_TRUE(set1.contains(4));
    EXPECT_TRUE(set1.contains(5));
    EXPECT_TRUE(set1.contains(6));
    EXPECT_TRUE(set1.contains(7));
}

using namespace postorder;

TEST(PostorderSetTest, InsertAndFind) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    EXPECT_TRUE(mySet.contains(5));
    EXPECT_TRUE(mySet.contains(3));
    EXPECT_TRUE(mySet.contains(7));
    EXPECT_FALSE(mySet.contains(10));
}

TEST(PostorderSetTest, SizeAndEmpty) {
    set<int> mySet;
    EXPECT_TRUE(mySet.empty());
    EXPECT_EQ(mySet.size(), 0);

    mySet.insert(5);
    EXPECT_FALSE(mySet.empty());
    EXPECT_EQ(mySet.size(), 1);
}

TEST(PostorderSetTest, Clear) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    mySet.clear();
    EXPECT_TRUE(mySet.empty());
    EXPECT_EQ(mySet.size(), 0);
}

TEST(PostorderSetTest, Iterator) {
    set<int> mySet;
    mySet.insert(5);
    mySet.insert(3);
    mySet.insert(7);

    std::vector<int> expectedValues = { 5, 3, 7 };
    std::vector<int> actualValues;
    for (auto it = mySet.begin(); it != mySet.end(); ++it) {
        actualValues.push_back(*it);
    }
    EXPECT_EQ(actualValues, expectedValues);
}

TEST(PostorderSetTest, CopyConstructorAndAssignment) {
    set<int> originalSet;
    originalSet.insert(5);
    originalSet.insert(3);
    originalSet.insert(7);

    set<int> copiedSet(originalSet);
    EXPECT_EQ(copiedSet.size(), 3);
    EXPECT_TRUE(copiedSet.contains(5));
    EXPECT_TRUE(copiedSet.contains(3));
    EXPECT_TRUE(copiedSet.contains(7));

    set<int> assignedSet;
    assignedSet = originalSet;
    EXPECT_EQ(assignedSet.size(), 3);
    EXPECT_TRUE(assignedSet.contains(5));
    EXPECT_TRUE(assignedSet.contains(3));
    EXPECT_TRUE(assignedSet.contains(7));
}

TEST(PostorderSetTest, Merge) {
    set<int> set1;
    set1.insert(5);
    set1.insert(3);
    set1.insert(7);

    set<int> set2;
    set2.insert(2);
    set2.insert(4);
    set2.insert(6);

    set1.merge(set2);
    EXPECT_EQ(set1.size(), 6);
    EXPECT_TRUE(set1.contains(2));
    EXPECT_TRUE(set1.contains(3));
    EXPECT_TRUE(set1.contains(4));
    EXPECT_TRUE(set1.contains(5));
    EXPECT_TRUE(set1.contains(6));
    EXPECT_TRUE(set1.contains(7));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
