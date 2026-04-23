#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Account.h"
#include <stdexcept>

class MockAccount : public Account {
public:
	MockAccount(int id, int balance) : Account(id, balance) {}

	MOCK_METHOD(int, GetBalance, (), (const, override));
	MOCK_METHOD(void, ChangeBalance, (int diff), (override));
	MOCK_METHOD(void, Lock, (), (override));
	MOCK_METHOD(void, Unlock, (), (override));
};

TEST(AccountTest, ConstructorAccount) {
	int id = 1000, balance = 10;
	Account acc(id, balance);
	EXPECT_EQ(acc.id(), 1000);
	EXPECT_EQ(acc.GetBalance(), 10);
}

TEST(AccountTest, DestructorAccount) {
	auto* acc = new Account(1000, 10);
	delete acc;
}

TEST(AccountTest, getBalance) {
	Account acc(1000, 10);
	EXPECT_EQ(acc.GetBalance(), 10);
}

TEST(AccountTest, Lock) {
	Account acc(1000, 10);
	EXPECT_NO_THROW(acc.Lock());
	EXPECT_THROW(acc.Lock(), std::runtime_error);
}

TEST(AccountTest, Unlock) {
	Account acc(1000, 10);
	acc.Lock();
	EXPECT_NO_THROW(acc.Unlock());
	EXPECT_NO_THROW(acc.Lock());
}

TEST(AccountTest, ChangeBalanceLock) {
	Account acc(1000, 10);
	acc.Lock();
	EXPECT_NO_THROW(acc.ChangeBalance(5));
	EXPECT_EQ(acc.GetBalance(), 15);
}

TEST(AccountTest, ChangeBalanceLockNeg) {
	Account acc(1000, 10);
	acc.Lock();
	EXPECT_NO_THROW(acc.ChangeBalance(-5));
	EXPECT_EQ(acc.GetBalance(), 5);	
}

TEST(AccountTest, ChangeBalanceUnlock) {
	Account acc(1000, 10);
	EXPECT_THROW(acc.ChangeBalance(5), std::runtime_error);
	EXPECT_EQ(acc.GetBalance(), 10);
}

TEST(MockAccountTest, TestLock) {
	MockAccount acc(1, 100);
	EXPECT_CALL(acc, Lock());
	acc.Lock();
}

TEST(MockAccountTest, TestUnlock) {
	MockAccount acc(1, 100);
	EXPECT_CALL(acc, Unlock());
	acc.Unlock();
}

TEST(MockAccountTest, TestGetBalance) {
	MockAccount acc(1, 100);
	EXPECT_CALL(acc, GetBalance()).WillOnce(testing::Return(100));
	EXPECT_EQ(acc.GetBalance(), 100);
}

TEST(MockAccountTest, TestChangeBalance) {
	MockAccount acc(1, 100);
	EXPECT_CALL(acc, ChangeBalance(50));
	acc.ChangeBalance(50);
}