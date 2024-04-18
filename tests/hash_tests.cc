#include <hash_table.h>
#include <gtest/gtest.h>
#include <iostream>

TEST(pair_test, constructor_params) {
	pair<int, int> pair(1, 2);
	ASSERT_EQ(*pair.key, 1);
	ASSERT_EQ(*pair.value, 2);
}
TEST(pair_test, copy_constructor) {
	pair<int, int> pair(1, 2);
	auto copy(pair);
	ASSERT_EQ(*copy.key, 1);
	ASSERT_EQ(*copy.value, 2);
}
TEST(pair_test, overloaded_eq) {
	pair<int, int> pair(1, 2);
	auto copy=pair;
	ASSERT_EQ(*copy.key, 1);
	ASSERT_EQ(*copy.value, 2);
}