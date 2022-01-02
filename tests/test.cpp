// Copyright 2021 GNDavydov


#include <gtest/gtest.h>
#include <string>

#include <hash_table.hpp>


TEST(HashTable, Init) {
    HashTable<int, double> hash_table1;
    HashTable<std::string, std::string> hash_table2;

    EXPECT_EQ(hash_table1.size(), 0);
    EXPECT_EQ(hash_table2.size(), 0);
    EXPECT_EQ(hash_table1.used_elements(), 0);
    EXPECT_EQ(hash_table2.used_elements(), 0);
}

TEST(HashTable, Insert) {
    HashTable<int, int> hash_table;

    EXPECT_NO_THROW(hash_table.insert(1, 1));
    EXPECT_EQ(hash_table.size(), 1);
    EXPECT_EQ(hash_table.used_elements(), 1);

    EXPECT_NO_THROW(hash_table.insert(1, 2));
    EXPECT_EQ(hash_table.size(), 2);
    EXPECT_EQ(hash_table.used_elements(), 2);

    EXPECT_NO_THROW(hash_table.insert(3, 4));
    EXPECT_EQ(hash_table.size(), 3);
    EXPECT_EQ(hash_table.used_elements(), 3);

    EXPECT_NO_THROW(hash_table.insert(56, 78));
    EXPECT_EQ(hash_table.size(), 4);
    EXPECT_EQ(hash_table.used_elements(), 4);

    EXPECT_EQ(true, hash_table.find(1));
    EXPECT_EQ(true, hash_table.find(3));
    EXPECT_EQ(true, hash_table.find(56));
}

TEST(HashTable, Remove) {
    HashTable<int, int> hash_table;

    hash_table.insert(1, 1);
    hash_table.insert(2, 3);
    hash_table.insert(3, 5);
    hash_table.insert(4, 7);
    hash_table.insert(5, 9);
    hash_table.insert(1, 11);

    EXPECT_EQ(hash_table.size(), 6);
    EXPECT_EQ(hash_table.used_elements(), 6);

    EXPECT_NO_THROW(hash_table.remove(3));
    EXPECT_EQ(hash_table.size(), 6);
    EXPECT_EQ(hash_table.used_elements(), 5);

    EXPECT_EQ(false, hash_table.find(3));

    EXPECT_NO_THROW(hash_table.remove(3));
    EXPECT_EQ(hash_table.size(), 6);
    EXPECT_EQ(hash_table.used_elements(), 5);

    EXPECT_NO_THROW(hash_table.remove(1));
    EXPECT_EQ(hash_table.size(), 6);
    EXPECT_EQ(hash_table.used_elements(), 4);

    EXPECT_EQ(true, hash_table.find(1));

    EXPECT_NO_THROW(hash_table.remove(1));
    EXPECT_EQ(hash_table.size(), 6);
    EXPECT_EQ(hash_table.used_elements(), 3);

    EXPECT_EQ(false, hash_table.find(1));

    hash_table.insert(1, 12);
    EXPECT_EQ(hash_table.size(), 6);
    EXPECT_EQ(hash_table.used_elements(), 4);
}


TEST(HashTable, Get) {
    HashTable<int, int> hash_table;

    hash_table.insert(1, 1);
    hash_table.insert(2, 3);
    hash_table.insert(3, 5);
    hash_table.insert(4, 7);
    hash_table.insert(5, 9);
    hash_table.insert(1, 11);

    EXPECT_EQ(hash_table.get(1), 1);
    EXPECT_EQ(hash_table.get(2), 3);
    EXPECT_EQ(hash_table.get(3), 5);
    EXPECT_EQ(hash_table.get(4), 7);
    EXPECT_EQ(hash_table.get(5), 9);

    hash_table.remove(1);
    EXPECT_EQ(hash_table.get(1), 11);

    hash_table.remove(1);
    EXPECT_THROW(hash_table.get(1), std::runtime_error);
}
