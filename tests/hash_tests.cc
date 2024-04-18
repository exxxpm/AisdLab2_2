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

TEST(hash_table_test, contains) {
	hash_table<int, int> ht(11, 2, 9);
	ht.insert_or_assign(505, 505);
	ASSERT_TRUE(ht.contains(505));
	ASSERT_FALSE(ht.contains(594590505));
}

TEST(hash_table_test, count) {
	hash_table<int, int> ht(11, 2, 9);
	ht.insert_or_assign(505, 505);
	ASSERT_EQ(ht.count(505),1);
	ASSERT_EQ(ht.count(594590505),0);
}

TEST(hash_table_test, searh) {
	hash_table<int, int> ht(11, 2, 9);
	ht.insert_or_assign(505, 505);
	ASSERT_EQ(*ht.search(505), 505);
}