// Created by Franz Seckel on 17.03.2026.
#include <doctest.h>
#include <fsd/core/vector.hpp>

TEST_CASE("Vector: default construction") {
    fsd::core::Vector<int> v;

    CHECK(v.size() == 0);
    CHECK(v.capacity() == 0);
    CHECK(v.empty());
    CHECK(v.full());
    CHECK(v.begin() == v.end());
}

TEST_CASE("Vector: initializer_list construction") {
    fsd::core::Vector<int> v{1, 2, 3};

    CHECK(v.size() == 3);
    CHECK(v.capacity() == 3);
    CHECK(v.front() == 1);
    CHECK(v.back() == 3);
    CHECK(v[1] == 2);
}

TEST_CASE("Vector: pushBack() grows and preserves values") {
    fsd::core::Vector<int> v;

    v.pushBack(10);
    v.pushBack(20);
    v.pushBack(30);

    CHECK(v.size() == 3);
    CHECK(v[0] == 10);
    CHECK(v[1] == 20);
    CHECK(v[2] == 30);
    CHECK_FALSE(v.empty());
}

TEST_CASE("Vector: emplaceBack() appends element") {
    fsd::core::Vector<int> v;

    v.emplaceBack(42);

    CHECK(v.size() == 1);
    CHECK(v.front() == 42);
    CHECK(v.back() == 42);
}

TEST_CASE("Vector: insertAt() inserts at arbitrary position") {
    fsd::core::Vector<int> v{1, 3, 4};

    v.insertAt(1, 2);

    CHECK(v.size() == 4);
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
    CHECK(v[2] == 3);
    CHECK(v[3] == 4);
}

TEST_CASE("Vector: removeAt() removes element and shifts tail") {
    fsd::core::Vector<int> v{1, 2, 3, 4};

    v.removeAt(1);

    CHECK(v.size() == 3);
    CHECK(v[0] == 1);
    CHECK(v[1] == 3);
    CHECK(v[2] == 4);
}

TEST_CASE("Vector: popBack() removes last element") {
    fsd::core::Vector<int> v{1, 2, 3};

    v.popBack();

    CHECK(v.size() == 2);
    CHECK(v.back() == 2);
}

TEST_CASE("Vector: reserve(n), resize(n) and shrinkToFit()") {
    fsd::core::Vector<int> v;

    v.reserve(16);
    CHECK(v.capacity() >= 16);

    v.resize(5);
    CHECK(v.size() == 5);

    v.shrinkToFit();
    CHECK(v.capacity() == v.size());
}

TEST_CASE("Vector: at() throws on invalid access") {
    fsd::core::Vector<int> v{1, 2, 3};

    CHECK_THROWS_AS(v.at(3), std::out_of_range);
    CHECK_THROWS_AS(v.insertAt(4, 99), std::out_of_range);
    CHECK_THROWS_AS(v.removeAt(3), std::out_of_range);
    CHECK_THROWS_AS(v.update(3, 99), std::out_of_range);
}

TEST_CASE("Vector: copy construction creates equal independent copy") {
    fsd::core::Vector<int> original{1, 2, 3};
    fsd::core::Vector<int> copy(original);

    CHECK(copy == original);

    copy.update(1, 99);

    CHECK(copy[1] == 99);
    CHECK(original[1] == 2);
    CHECK(copy != original);
}

TEST_CASE("Vector: move construction transfers contents") {
    fsd::core::Vector<int> original{1, 2, 3};

    fsd::core::Vector<int> moved(std::move(original));

    CHECK(moved.size() == 3);
    CHECK(moved[0] == 1);
    CHECK(moved[1] == 2);
    CHECK(moved[2] == 3);

    CHECK(original.size() == 0);
    CHECK(original.capacity() == 0);
}

TEST_CASE("Vector: find returns iterator to element or end") {
    fsd::core::Vector<int> v{5, 10, 15};

    auto it = v.find(10);
    auto missing = v.find(99);

    CHECK(it != v.end());
    CHECK(*it == 10);
    CHECK(missing == v.end());
}

TEST_CASE("Vector: comparison operators are lexicographical") {
    fsd::core::Vector<int> a{1, 2, 3};
    fsd::core::Vector<int> b{1, 2, 3};
    fsd::core::Vector<int> c{1, 2, 4};

    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
    CHECK(c > a);
    CHECK(a <= b);
    CHECK(c >= a);
}