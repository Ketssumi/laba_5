#include <gtest/gtest.h>
#include "my_memory_resource.hpp"
#include "queue.h"

class QueueTest : public ::testing::Test
{
protected:
    MyMemoryResource resource{4096};
};

TEST_F(QueueTest, EmptyQueueOnCreation)
{
    Queue<int> q(&resource);
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0);
}

TEST_F(QueueTest, PushAndSize)
{
    Queue<int> q(&resource);
    q.push(10);
    q.push(20);
    EXPECT_EQ(q.size(), 2);
}

TEST_F(QueueTest, PopReturnsCorrectOrder)
{
    Queue<int> q(&resource);
    q.push(1);
    q.push(2);
    q.push(3);

    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 3);
}

TEST_F(QueueTest, FrontReturnsFirstElement)
{
    Queue<int> q(&resource);
    q.push(10);
    q.push(20);
    EXPECT_EQ(q.front(), 10);
}

TEST_F(QueueTest, PopOnEmptyThrows)
{
    Queue<int> q(&resource);
    EXPECT_THROW(q.pop(), std::underflow_error);
}

TEST_F(QueueTest, FrontOnEmptyThrows)
{
    Queue<int> q(&resource);
    EXPECT_THROW(q.front(), std::underflow_error);
}

TEST_F(QueueTest, IteratorTraversesInOrder)
{
    Queue<int> q(&resource);
    q.push(1);
    q.push(2);
    q.push(3);

    int expected = 1;
    for (int value : q)
    {
        EXPECT_EQ(value, expected);
        expected++;
    }
}

TEST_F(QueueTest, QueueWithStructType)
{
    struct Item
    {
        int id;
        double value;
    };

    Queue<Item> q(&resource);
    q.push({1, 1.5});
    q.push({2, 2.5});

    Item first = q.pop();
    EXPECT_EQ(first.id, 1);
    EXPECT_DOUBLE_EQ(first.value, 1.5);
}

TEST_F(QueueTest, MemoryReuseAfterDeallocate)
{
    Queue<int> q(&resource);
    q.push(100);
    q.push(200);

    int val1 = q.pop();
    int val2 = q.pop();

    EXPECT_EQ(val1, 100);
    EXPECT_EQ(val2, 200);
    EXPECT_TRUE(q.empty());

    // Reuse same queue after emptying
    q.push(300);
    EXPECT_EQ(q.front(), 300);
}

TEST_F(QueueTest, MultipleIterations)
{
    Queue<int> q(&resource);
    q.push(1);
    q.push(2);
    q.push(3);

    // First iteration
    int count1 = 0;
    for (int v : q)
        count1++;

    // Second iteration
    int count2 = 0;
    for (int v : q)
        count2++;

    EXPECT_EQ(count1, 3);
    EXPECT_EQ(count2, 3);
}

TEST_F(QueueTest, IteratorInequality)
{
    Queue<int> q(&resource);
    q.push(10);
    q.push(20);

    auto it1 = q.begin();
    auto it2 = q.begin();
    auto end = q.end();

    EXPECT_EQ(it1, it2);
    EXPECT_NE(it1, end);
}

TEST_F(QueueTest, ComplexOperationSequence)
{
    Queue<int> q(&resource);

    for (int i = 0; i < 10; i++)
    {
        q.push(i);
    }
    EXPECT_EQ(q.size(), 10);

    for (int i = 0; i < 5; i++)
    {
        q.pop();
    }
    EXPECT_EQ(q.size(), 5);

    q.push(100);
    EXPECT_EQ(q.size(), 6);
}
