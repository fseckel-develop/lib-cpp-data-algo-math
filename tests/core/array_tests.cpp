// Created by Franz Seckel on 17.03.2026.
#include <doctest.h>
#include <fsd/core/array.hpp>

TEST_CASE("Array: default construction initializes with fixed size") {
    fsd::core::Array<int, 4> a;

    CHECK(a.size() == 4);
    CHECK(a.maxSize() == 4);
    CHECK_FALSE(a.empty());

    CHECK(a[0] == 0);
    CHECK(a[1] == 0);
    CHECK(a[2] == 0);
    CHECK(a[3] == 0);
}

TEST_CASE("Array: initializer_list construction copies provided values") {
    fsd::core::Array<int, 4> a{1, 2, 3};

    CHECK(a.size() == 4);
    CHECK(a[0] == 1);
    CHECK(a[1] == 2);
    CHECK(a[2] == 3);
    CHECK(a[3] == 0);
}

TEST_CASE("Array: front(), back() and data() provide element access") {
    fsd::core::Array<int, 3> a{10, 20, 30};

    CHECK(a.front() == 10);
    CHECK(a.back() == 30);
    CHECK(a.data() != nullptr);
    CHECK(a.data()[1] == 20);
}

TEST_CASE("Array: at() throws on invalid access") {
    fsd::core::Array<int, 3> a{1, 2, 3};

    CHECK_NOTHROW(a.at(0));
    CHECK_NOTHROW(a.at(2));
    CHECK_THROWS_AS(a.at(3), std::out_of_range);
}

TEST_CASE("Array: fill() assigns all elements") {
    fsd::core::Array<int, 5> a{1, 2, 3, 4, 5};

    a.fill(9);

    for (std::size_t i = 0; i < a.size(); ++i) {
        CHECK(a[i] == 9);
    }
}

TEST_CASE("Array: swap() exchanges contents") {
    fsd::core::Array<int, 3> a{1, 2, 3};
    fsd::core::Array<int, 3> b{4, 5, 6};

    a.swap(b);

    CHECK(a[0] == 4);
    CHECK(a[1] == 5);
    CHECK(a[2] == 6);

    CHECK(b[0] == 1);
    CHECK(b[1] == 2);
    CHECK(b[2] == 3);
}

TEST_CASE("Array: comparison operators are lexicographical") {
    fsd::core::Array<int, 3> a{1, 2, 3};
    fsd::core::Array<int, 3> b{1, 2, 3};
    fsd::core::Array<int, 3> c{1, 2, 4};

    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
    CHECK(c > a);
    CHECK(a <= b);
    CHECK(c >= a);
}

TEST_CASE("Array: iterators traverse elements in order") {
    fsd::core::Array<int, 4> a{1, 2, 3, 4};

    int expected = 1;
    for (auto it = a.begin(); it != a.end(); ++it) {
        CHECK(*it == expected);
        ++expected;
    }
}

TEST_CASE("Array: reverse iterators traverse elements in reverse order") {
    fsd::core::Array<int, 4> a{1, 2, 3, 4};

    int expected = 4;
    for (auto it = a.rbegin(); it != a.rend(); ++it) {
        CHECK(*it == expected);
        --expected;
    }
}