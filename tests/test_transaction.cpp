#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>
#include "Transaction.h"
#include "Account.h"

using ::testing::NiceMock;

class MockAccount : public Account {
public:
	MockAccount(int id, int balance) : Account(id, balance) {}

	MOCK_METHOD(int, GetBalance, (), (const, override));
	MOCK_METHOD(void, ChangeBalance, (int diff), (override));
	MOCK_METHOD(void, Lock, (), (override));
	MOCK_METHOD(void, Unlock, (), (override));
};

TEST(TransactionTest, SuccessfulTransaction) {
	Transaction trn;
	trn.set_fee(10);
	Account from(1, 500);
	Account to(2, 100);

	bool res = trn.Make(from, to, 200);

	EXPECT_TRUE(res);
	EXPECT_EQ(from.GetBalance(), 500);
	EXPECT_EQ(to.GetBalance(), 90);
}

TEST(TransactionTest, FailedTransaction) {
	Transaction trn;
	trn.set_fee(10);
	Account from(1,  500);
	Account to(2, 10);

	bool res = trn.Make(from, to, 200);

	EXPECT_FALSE(res);
	EXPECT_EQ(from.GetBalance(), 500);
	EXPECT_EQ(to.GetBalance(), 10);
}

TEST(TransactionTest, SumLess100) {
	Transaction trn;
	Account from(1, 500);
	Account to(2, 100);
	EXPECT_THROW(trn.Make(from, to, 50), std::logic_error);
}

TEST(TransactionTest, SameAccount) {
	Transaction trn;
	Account acc(1, 500);
	EXPECT_THROW(trn.Make(acc, acc, 200), std::logic_error);
}

TEST(TransactionTest, FeeTooHigh) {
	Transaction trn;
	trn.set_fee(100);
	Account from(1, 500);
	Account to(2, 100);
	EXPECT_FALSE(trn.Make(from, to, 150));
}

TEST(TransactionMockTest, ValidTransaction) {
	NiceMock<MockAccount> from(1, 500);
	NiceMock<MockAccount> to(2, 100);
	Transaction trn;
	trn.set_fee(10);

	EXPECT_CALL(from, Lock()).Times(1);
	EXPECT_CALL(to, Lock()).Times(1);
	EXPECT_CALL(from, Unlock()).Times(1);
	EXPECT_CALL(to, Unlock()).Times(1);

	EXPECT_CALL(to, ChangeBalance(200)).Times(1);
	EXPECT_CALL(to, GetBalance()).WillRepeatedly(testing::Return(300));
	EXPECT_CALL(to, ChangeBalance(-210)).Times(1);

	trn.Make(from, to, 200);
}

TEST(TransactionMockTest, MockSumLess100) {
	NiceMock<MockAccount> from(1, 500);
	NiceMock<MockAccount> to(2, 100);
	Transaction trn;

	EXPECT_CALL(from, Lock()).Times(0);
	EXPECT_CALL(to, Lock()).Times(0);

	EXPECT_THROW(trn.Make(from, to, 50), std::logic_error);
}

TEST(TransactionMockTest, FeeTooHighMock) {
	NiceMock<MockAccount> from(1, 500);
	NiceMock<MockAccount> to(2, 100);
	Transaction trn;
	trn.set_fee(100);

	EXPECT_CALL(from, Lock()).Times(0);
	EXPECT_CALL(to, Lock()).Times(0);

	trn.Make(from, to, 150);
}

TEST(TransactionMockTest, DebitFails) {
	NiceMock<MockAccount> from(1, 500);
	NiceMock<MockAccount> to(2, 10);
	Transaction trn;
	trn.set_fee(10);

	EXPECT_CALL(to, ChangeBalance(200)).Times(1);
	EXPECT_CALL(to, GetBalance()).WillRepeatedly(testing::Return(210));
	EXPECT_CALL(to, ChangeBalance(-200)).Times(1);

	trn.Make(from, to, 200);
}