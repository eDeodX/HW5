#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>
#include <sstream>
#include "Transaction.h"
#include "Account.h"
#include <iostream>
#include <string>

class MockTransaction : public Transaction {
public:
	MockTransaction() : Transaction() {}

	MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(TransactionTest, SaveToDataBaseSucces) {
	MockTransaction trn;
	trn.set_fee(10);

	Account from(1, 500);
	Account to(2, 100);

	EXPECT_CALL(trn, SaveToDataBase(::testing::Ref(from), ::testing::Ref(to), 200)).Times(1);

	trn.Make(from, to, 200);
}

TEST(TransactionTest, SaveToDataBaseFail) {
	MockTransaction trn;
	trn.set_fee(100);

	Account from(1, 500);
	Account to(2, 100);

	EXPECT_CALL(trn, SaveToDataBase(::testing::_, ::testing::_, ::testing::_)).Times(0);

	trn.Make(from, to, 150);
}

TEST(TransactionTest, SaveToDataBaseCorrectSum) {
	MockTransaction trn;
	trn.set_fee(5);

	Account from(1, 1000);
	Account to(2, 200);

	EXPECT_CALL(trn, SaveToDataBase(::testing::_, ::testing::_, 300)).Times(1);

	trn.Make(from, to, 300);
}

TEST(TransactionMakeTest, ID1EqualsID2) {
	Transaction trn;
	Account acc1(1, 100);
	Account acc2(1, 200);
	EXPECT_THROW(trn.Make(acc1, acc2, 200), std::logic_error);
}

TEST(TransactionMakeTest, SumNeg) {
	Transaction trn;
	Account from(1, 100);
	Account to(2, 50);

	EXPECT_THROW(trn.Make(from, to, -50), std::invalid_argument);
}

TEST(TransactionMakeTest, SumLess100) {
	Transaction trn;
	Account from(1, 100);
	Account to(2, 50);
	EXPECT_THROW(trn.Make(from, to, 99), std::logic_error);
}

TEST(TransactionMakeTest, FeeGreaterSum) {
	Transaction trn;
	trn.set_fee(100);
	Account from(1, 1000);
	Account to(2, 500);

	bool result = trn.Make(from, to, 150);
	EXPECT_FALSE(result);
}

TEST(TransactionMakeTest, Succesful) {
	Transaction trn;
	trn.set_fee(10);
	Account from(1, 500);
	Account to(2, 100);

	bool result = trn.Make(from, to, 200);
	EXPECT_TRUE(result);

	EXPECT_EQ(from.GetBalance(), 500);
	EXPECT_EQ(to.GetBalance(), 90);
}

TEST(TransactionMakeTest, DebitReturnFalse) {
	Transaction trn;
	trn.set_fee(10);
	Account from(1, 500);
	Account to(2, 10);

	bool result = trn.Make(from, to, 200);
	EXPECT_FALSE(result);

	EXPECT_EQ(from.GetBalance(), 500);
	EXPECT_EQ(to.GetBalance(), 10);
}

TEST(TransactionMakeTest, FeeTooHigh) {
	Transaction trn;
	trn.set_fee(100);
	Account from(1, 1000);
	Account to(2, 500);

	bool result = trn.Make(from, to, 199);
	EXPECT_FALSE(result);
	EXPECT_EQ(from.GetBalance(), 1000);
	EXPECT_EQ(to.GetBalance(), 500);
}

TEST(TransactionTest, SetterGetter) {
	Transaction trn;
	EXPECT_EQ(trn.fee(), 1);

	trn.set_fee(50);
	EXPECT_EQ(trn.fee(), 50);

	trn.set_fee(0);
	EXPECT_EQ(trn.fee(), 0);
}