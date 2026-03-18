// Created by Franz Seckel on 17.03.2026.
#include <doctest.h>
#include <fsd/core/deque.hpp>

TEST_CASE("Deque: default construction creates empty deque") {
    fsd::core::Deque<int> d;

    CHECK(d.size() == 0);
    CHECK(d.empty());
}

TEST_CASE("Deque: initializer_list constructor preserves element order") {
    fsd::core::Deque<int> d{1, 2, 3};

    CHECK(d.size() == 3);
    CHECK_FALSE(d.empty());
    CHECK(d.front() == 1);
    CHECK(d.back() == 3);
    CHECK(d[0] == 1);
    CHECK(d[1] == 2);
    CHECK(d[2] == 3);
}

TEST_CASE("Deque: pushBack() appends elements to the back") {
    fsd::core::Deque<int> d;

    d.pushBack(10);
    d.pushBack(20);
    d.pushBack(30);

    CHECK(d.size() == 3);
    CHECK(d.front() == 10);
    CHECK(d.back() == 30);
    CHECK(d[0] == 10);
    CHECK(d[1] == 20);
    CHECK(d[2] == 30);
}

TEST_CASE("Deque: pushFront() prepends elements to the front") {
    fsd::core::Deque<int> d;

    d.pushFront(10);
    d.pushFront(20);
    d.pushFront(30);

    CHECK(d.size() == 3);
    CHECK(d.front() == 30);
    CHECK(d.back() == 10);
    CHECK(d[0] == 30);
    CHECK(d[1] == 20);
    CHECK(d[2] == 10);
}

TEST_CASE("Deque: pushFront() and pushBack() support mixed insertion order") {
    fsd::core::Deque<int> d;

    d.pushBack(2);
    d.pushBack(3);
    d.pushFront(1);
    d.pushBack(4);
    d.pushFront(0);

    CHECK(d.size() == 5);
    CHECK(d[0] == 0);
    CHECK(d[1] == 1);
    CHECK(d[2] == 2);
    CHECK(d[3] == 3);
    CHECK(d[4] == 4);
    CHECK(d.front() == 0);
    CHECK(d.back() == 4);
}

TEST_CASE("Deque: popFront() removes elements from the front") {
    fsd::core::Deque<int> d{1, 2, 3};

    d.popFront();

    CHECK(d.size() == 2);
    CHECK(d.front() == 2);
    CHECK(d.back() == 3);
    CHECK(d[0] == 2);
    CHECK(d[1] == 3);
}

TEST_CASE("Deque: popBack() removes elements from the back") {
    fsd::core::Deque<int> d{1, 2, 3};

    d.popBack();

    CHECK(d.size() == 2);
    CHECK(d.front() == 1);
    CHECK(d.back() == 2);
    CHECK(d[0] == 1);
    CHECK(d[1] == 2);
}

TEST_CASE("Deque: popFront() and popBack() on empty deque leave it unchanged") {
    fsd::core::Deque<int> d;

    d.popFront();
    d.popBack();

    CHECK(d.size() == 0);
    CHECK(d.empty());
}

TEST_CASE("Deque: at() throws on out-of-bounds access") {
    fsd::core::Deque<int> d{1, 2, 3};

    CHECK_NOTHROW(d.at(0));
    CHECK_NOTHROW(d.at(2));
    CHECK_THROWS_AS(d.at(3), std::out_of_range);
}

TEST_CASE("Deque: clear() removes all elements") {
    fsd::core::Deque<int> d{1, 2, 3, 4};

    d.clear();

    CHECK(d.size() == 0);
    CHECK(d.empty());
}

TEST_CASE("Deque: copy construction creates equal independent copy") {
    fsd::core::Deque<int> original{1, 2, 3};
    fsd::core::Deque<int> copy(original);

    CHECK(copy == original);

    copy.popBack();
    copy.pushBack(99);

    CHECK(copy.size() == 3);
    CHECK(original.size() == 3);
    CHECK(copy.back() == 99);
    CHECK(original.back() == 3);
    CHECK(copy != original);
}

TEST_CASE("Deque: move construction transfers contents") {
    fsd::core::Deque<int> original{1, 2, 3};

    fsd::core::Deque<int> moved(std::move(original));

    CHECK(moved.size() == 3);
    CHECK(moved.front() == 1);
    CHECK(moved.back() == 3);
    CHECK(moved[0] == 1);
    CHECK(moved[1] == 2);
    CHECK(moved[2] == 3);
}

TEST_CASE("Deque: swap() exchanges contents") {
    fsd::core::Deque<int> a{1, 2, 3};
    fsd::core::Deque<int> b{10, 20};

    a.swap(b);

    CHECK(a.size() == 2);
    CHECK(a.front() == 10);
    CHECK(a.back() == 20);

    CHECK(b.size() == 3);
    CHECK(b.front() == 1);
    CHECK(b.back() == 3);
}

TEST_CASE("Deque: iterators traverse elements in forward order") {
    fsd::core::Deque<int> d{1, 2, 3, 4};

    int expected = 1;
    for (auto it = d.begin(); it != d.end(); ++it) {
        CHECK(*it == expected);
        ++expected;
    }
}

TEST_CASE("Deque: reverse iterators traverse elements in reverse order") {
    fsd::core::Deque<int> d{1, 2, 3, 4};

    int expected = 4;
    for (auto it = d.rbegin(); it != d.rend(); ++it) {
        CHECK(*it == expected);
        --expected;
    }
}

TEST_CASE("Deque: comparison operators are lexicographical") {
    fsd::core::Deque<int> a{1, 2, 3};
    fsd::core::Deque<int> b{1, 2, 3};
    fsd::core::Deque<int> c{1, 2, 4};

    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
    CHECK(c > a);
    CHECK(a <= b);
    CHECK(c >= a);
}