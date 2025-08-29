#include <gtest/gtest.h>
#include <algorithm>
#include <iterator>
#include "CyclicList.hpp"

// Test fixture for CyclicList tests
class CyclicListTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup code for all tests
        list = {1, 2, 3, 4, 5};
    }
    
    CyclicList<int> list;
    CyclicList<int> emptyList;
};

// Test default constructor
TEST_F(CyclicListTest, DefaultConstructorCreatesEmptyList) {
    CyclicList<int> newList;
    EXPECT_TRUE(newList.empty());
    EXPECT_EQ(newList.size(), 0);
}

// Test initializer list constructor
TEST_F(CyclicListTest, InitializerListConstructorPopulatesList) {
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 5);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 5);
}

// Test copy constructor
TEST_F(CyclicListTest, CopyConstructorCreatesIndependentCopy) {
    CyclicList<int> copyList(list);
    
    EXPECT_EQ(copyList.size(), list.size());
    EXPECT_EQ(copyList.front(), list.front());
    EXPECT_EQ(copyList.back(), list.back());
    
    // Modify original, ensure copy is unaffected
    list.push_back(6);
    EXPECT_EQ(list.size(), 6);
    EXPECT_EQ(copyList.size(), 5);
}

// Test move constructor
TEST_F(CyclicListTest, MoveConstructorTransfersOwnership) {
    CyclicList<int> movedList(std::move(list));
    
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(movedList.size(), 5);
    EXPECT_EQ(movedList.front(), 1);
    EXPECT_EQ(movedList.back(), 5);
}

// Test copy assignment operator
TEST_F(CyclicListTest, CopyAssignmentCreatesIndependentCopy) {
    CyclicList<int> copyList;
    copyList = list;
    
    EXPECT_EQ(copyList.size(), list.size());
    EXPECT_EQ(copyList.front(), list.front());
    EXPECT_EQ(copyList.back(), list.back());
    
    // Modify original, ensure copy is unaffected
    list.push_back(6);
    EXPECT_EQ(list.size(), 6);
    EXPECT_EQ(copyList.size(), 5);
}

// Test move assignment operator
TEST_F(CyclicListTest, MoveAssignmentTransfersOwnership) {
    CyclicList<int> movedList;
    movedList = std::move(list);
    
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(movedList.size(), 5);
    EXPECT_EQ(movedList.front(), 1);
    EXPECT_EQ(movedList.back(), 5);
}

// Test empty() and size() methods
TEST_F(CyclicListTest, EmptyAndSizeMethodsWorkCorrectly) {
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 5);
    
    EXPECT_TRUE(emptyList.empty());
    EXPECT_EQ(emptyList.size(), 0);
}

// Test front() and back() methods
TEST_F(CyclicListTest, FrontAndBackMethodsReturnCorrectElements) {
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 5);
    
    // Test with single element
    CyclicList<int> singleElementList = {42};
    EXPECT_EQ(singleElementList.front(), 42);
    EXPECT_EQ(singleElementList.back(), 42);
}

// Test push_front() and push_back() methods
TEST_F(CyclicListTest, PushFrontAndPushBackAddElementsCorrectly) {
    list.push_front(0);
    list.push_back(6);
    
    EXPECT_EQ(list.front(), 0);
    EXPECT_EQ(list.back(), 6);
    EXPECT_EQ(list.size(), 7);
}

// Test pop_front() and pop_back() methods
TEST_F(CyclicListTest, PopFrontAndPopBackRemoveElementsCorrectly) {
    list.pop_front();
    list.pop_back();
    
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.back(), 4);
    EXPECT_EQ(list.size(), 3);
}

// Test clear() method
TEST_F(CyclicListTest, ClearMethodRemovesAllElements) {
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

// Test iterator functionality
TEST_F(CyclicListTest, IteratorsAllowForwardTraversal) {
    int expected[] = {1, 2, 3, 4, 5};
    int i = 0;
    
    for (auto it = list.begin(); it != list.end(); ++it) {
        EXPECT_EQ(*it, expected[i++]);
    }
}

// Test const iterator functionality
TEST_F(CyclicListTest, ConstIteratorsAllowForwardTraversal) {
    int expected[] = {1, 2, 3, 4, 5};
    int i = 0;
    
    for (auto it = list.cbegin(); it != list.cend(); ++it) {
        EXPECT_EQ(*it, expected[i++]);
    }
}

// Test reverse iterator functionality
TEST_F(CyclicListTest, IteratorsAllowReverseTraversal) {
    int expected[] = {5, 4, 3, 2, 1};
    int i = 0;
    
    auto it = list.end();
    do {
        --it;
        EXPECT_EQ(*it, expected[i++]);
    } while (it != list.begin());
}

// Test range-based for loop compatibility
TEST_F(CyclicListTest, RangeBasedForLoopWorksCorrectly) {
    int expected[] = {1, 2, 3, 4, 5};
    int i = 0;
    
    for (const auto& element : list) {
        EXPECT_EQ(element, expected[i++]);
    }
}

// Test insert() method
TEST_F(CyclicListTest, InsertAddsElementAtSpecifiedPosition) {
    auto it = list.begin();
    std::advance(it, 2); // Move to position of element 3
    
    auto newIt = list.insert(it, 99);
    EXPECT_EQ(*newIt, 99);
    EXPECT_EQ(list.size(), 6);
    
    // Check the elements are in correct order
    int expected[] = {1, 2, 99, 3, 4, 5};
    int i = 0;
    for (const auto& element : list) {
        EXPECT_EQ(element, expected[i++]);
    }
}

// Test erase() method
TEST_F(CyclicListTest, EraseRemovesElementAtSpecifiedPosition) {
    auto it = list.begin();
    std::advance(it, 2); // Move to position of element 3
    
    auto newIt = list.erase(it);
    EXPECT_EQ(*newIt, 4);
    EXPECT_EQ(list.size(), 4);
    
    // Check the elements are in correct order
    int expected[] = {1, 2, 4, 5};
    int i = 0;
    for (const auto& element : list) {
        EXPECT_EQ(element, expected[i++]);
    }
}

// Test swap() method
TEST_F(CyclicListTest, SwapExchangesContentsBetweenLists) {
    CyclicList<int> otherList = {10, 20, 30};
    
    list.swap(otherList);
    
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.back(), 30);
    
    EXPECT_EQ(otherList.size(), 5);
    EXPECT_EQ(otherList.front(), 1);
    EXPECT_EQ(otherList.back(), 5);
}

// Test compatibility with STL algorithms
TEST_F(CyclicListTest, WorksWithStdFindAlgorithm) {
    auto it = std::find(list.begin(), list.end(), 3);
    EXPECT_NE(it, list.end());
    EXPECT_EQ(*it, 3);
    
    it = std::find(list.begin(), list.end(), 99);
    EXPECT_EQ(it, list.end());
}

// Test compatibility with std::copy algorithm
TEST_F(CyclicListTest, WorksWithStdCopyAlgorithm) {
    std::vector<int> vec;
    std::copy(list.begin(), list.end(), std::back_inserter(vec));
    
    EXPECT_EQ(vec.size(), list.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        auto listIt = list.begin();
        std::advance(listIt, i);
        EXPECT_EQ(vec[i], *listIt);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
