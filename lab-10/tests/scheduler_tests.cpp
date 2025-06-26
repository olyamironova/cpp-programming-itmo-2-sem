#include <gtest/gtest.h>

#include "../lib/scheduler.h"


class TTaskSchedulerTest : public ::testing::Test {
protected:
    TTaskScheduler scheduler;
};

TEST_F(TTaskSchedulerTest, AddFunctionNoArgs) {
    auto id = scheduler.add([]() { return 24; });
    EXPECT_EQ(scheduler.getResult<int>(id), 24);
}

TEST_F(TTaskSchedulerTest, GetFutureResult) {
    auto id = scheduler.add([](int a, int b) { return a + b; }, 10, 14);
    auto future = scheduler.getFutureResult<int>(id);
    EXPECT_EQ(future, 24);
}

TEST_F(TTaskSchedulerTest, ExecuteAll) {
    auto id1 = scheduler.add([]() { return 10; });
    auto id2 = scheduler.add([]() { return 14; });
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<int>(id1), 10);
    EXPECT_EQ(scheduler.getResult<int>(id2), 14);
}


TEST_F(TTaskSchedulerTest, AddFunctionOneArg) {
    auto id = scheduler.add([](int a) { return a * 2; }, 12);
    EXPECT_EQ(scheduler.getResult<int>(id), 24);
}

TEST_F(TTaskSchedulerTest, AddFunctionTwoArgs) {
    auto id = scheduler.add([](int a, int b) { return a + b; }, 10, 14);
    EXPECT_EQ(scheduler.getResult<int>(id), 24);
}

TEST_F(TTaskSchedulerTest, GetFutureResultBeforeExecute) {
    auto id = scheduler.add([]() { return 24; });
    auto future = scheduler.getFutureResult<int>(id);
    EXPECT_EQ(future, 24);
}

TEST_F(TTaskSchedulerTest, ExecuteAllWithDependencies) {
    auto id1 = scheduler.add([]() { return 10; });
    auto id2 = scheduler.add([](int a) { return a + 4; }, scheduler.getFutureResult<int>(id1));
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<int>(id1), 10);
    EXPECT_EQ(scheduler.getResult<int>(id2), 14);
}

TEST_F(TTaskSchedulerTest, ExecuteAllWithMultipleDependencies) {
    auto id1 = scheduler.add([]() { return 10; });
    auto id2 = scheduler.add([](int a) { return a + 2; }, scheduler.getFutureResult<int>(id1));
    auto id3 = scheduler.add([](int a) { return a + 2; }, scheduler.getFutureResult<int>(id2));
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<int>(id1), 10);
    EXPECT_EQ(scheduler.getResult<int>(id2), 12);
    EXPECT_EQ(scheduler.getResult<int>(id3), 14);
}

TEST_F(TTaskSchedulerTest, GetResultAfterExecute) {
    auto id = scheduler.add([]() { return 24; });
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<int>(id), 24);
}

TEST_F(TTaskSchedulerTest, GetFutureResultAfterExecute) {
    auto id = scheduler.add([]() { return 24; });
    scheduler.executeAll();
    auto future = scheduler.getFutureResult<int>(id);
    EXPECT_EQ(future, 24);
}


TEST_F(TTaskSchedulerTest, AddFunctionWithLambdaCapture) {
    int a = 24;
    auto id = scheduler.add([a]() { return a; });
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<int>(id), 24);
}


TEST_F(TTaskSchedulerTest, AddFunctionWithReferenceArgument) {
    int a = 24;
    auto id = scheduler.add([](const int& a) { return a; }, a);
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<int>(id), 24);
}

TEST_F(TTaskSchedulerTest, AddFunctionWithReferenceDependency) {
    int a = 24;
    auto id1 = scheduler.add([](const int& a) { return a; }, a);
    auto id2 = scheduler.add([](const int& a) { return a + 1; }, scheduler.getFutureResult<int>(id1));
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<int>(id1), 24);
    EXPECT_EQ(scheduler.getResult<int>(id2), 25);
}

TEST_F(TTaskSchedulerTest, AddFunctionNoArgsChar) {
    auto id = scheduler.add([]() { return 'x'; });
    EXPECT_EQ(scheduler.getResult<char>(id), 'x');
}

TEST_F(TTaskSchedulerTest, ExecuteAllChar) {
    auto id1 = scheduler.add([]() { return 'x'; });
    auto id2 = scheduler.add([]() { return 'y'; });
    scheduler.executeAll();
    EXPECT_EQ(scheduler.getResult<char>(id1), 'x');
    EXPECT_EQ(scheduler.getResult<char>(id2), 'y');
}
