#include <hash_table.h>
#include <gtest/gtest.h>
#include <iostream>

TEST(pair_test, constructor_params) {
	pair<int, int> pair(1, 2);
	ASSERT_EQ(pair.key, 1);
	ASSERT_EQ(pair.value, 2);
}

TEST(hash_table_test, default_constr) {
	hash_table<int,int> ht(11);
	ht.insert(2, 5);
	ASSERT_EQ(ht.get_size(),1);
}

TEST(hash_table_test, rand_constr) {
	hash_table<int, int> ht(11,2,9);
	ASSERT_EQ(ht.get_size(), 11);
}

TEST(hash_table_test, insert) {
	hash_table<int, int> ht(11, 2, 9);
	ht.insert(66, 7);
	ASSERT_EQ(ht.get_size(), 12);
}

TEST(hash_table_test, assign_with_insert_or_assign) {
	hash_table<int, int> ht(11, 2, 9);
	ht.insert_or_assign(7, 505);
	ASSERT_EQ(ht.get_size(), 11);
}

TEST(hash_table_test, insert_with_insert_or_assign) {
	hash_table<int, int> ht(11, 2, 9);
	ht.insert_or_assign(505, 505);
	ASSERT_EQ(ht.get_size(), 12);
}