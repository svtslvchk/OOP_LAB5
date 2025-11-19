#include "gtest/gtest.h"
#include "../includes/memory_pool_resourse.hpp"
#include "../includes/pmr_vector.hpp"



TEST(PmrVectorTest, InitiallyEmpty) {
    memory_pool_resourse r;
    pmr_vector<int> v(&r);

    ASSERT_TRUE(v.empty());
    ASSERT_EQ(v.size(), 0);
}

TEST(PmrVectorTest, PushBackWorks) {
    memory_pool_resourse r;
    pmr_vector<int> v(&r);

    v.push_back(10);
    v.push_back(20);

    ASSERT_EQ(v.size(), 2);
    ASSERT_EQ(v[0], 10);
    ASSERT_EQ(v[1], 20);
}

TEST(PmrVectorTest, PopBackWorks) {
    memory_pool_resourse r;
    pmr_vector<int> v(&r);

    v.push_back(3);
    v.push_back(7);
    v.pop_back();

    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v[0], 3);
}

TEST(PmrVectorTest, IteratorWorks) {
    memory_pool_resourse r;
    pmr_vector<int> v(&r);

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    std::vector<int> out;
    for (int x : v) out.push_back(x);

    ASSERT_EQ(out.size(), 3);
    ASSERT_EQ(out[0], 1);
    ASSERT_EQ(out[1], 2);
    ASSERT_EQ(out[2], 3);
}

struct Obj {
    int id;
    std::string name;

    Obj(int i, std::string n) : id(i), name(std::move(n)) {}
};

TEST(PmrVectorTest, ComplexTypeWorks) {
    memory_pool_resourse r;
    pmr_vector<Obj> v(&r);

    v.emplace_back(1, "one");
    v.emplace_back(2, "two");

    ASSERT_EQ(v[0].name, "one");
    ASSERT_EQ(v[1].name, "two");
}
