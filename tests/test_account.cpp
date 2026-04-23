#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Account.h"
#include <stdexcept>

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