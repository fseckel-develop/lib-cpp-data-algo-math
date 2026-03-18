// Created by Franz Seckel on 18.03.2026.
#include <doctest.h>
#include <fsd/core/double_list.hpp>

TEST_CASE("DoubleList: default construction creates empty list") {
    fsd::core::DoubleList<int> list;

    CHECK(list.size() == 0);
    CHECK(list.empty());
    CHECK(list.begin() == list.end());
    CHECK(list.rbegin() == list.rend());
}

TEST_CASE("DoubleList: initializer_list constructor preserves element order") {
    fsd::core::DoubleList<int> list{1, 2, 3};

    CHECK(list.size() == 3);
    CHECK_FALSE(list.empty());
    CHECK(list.front() == 1);
    CHECK(list.back() == 3);
    CHECK(list[0] == 1);
    CHECK(list[1] == 2);
    CHECK(list[2] == 3);
}

TEST_CASE("DoubleList: pushFront() prepends elements to the front") {
    fsd::core::DoubleList<int> list;

    list.pushFront(10);
    list.pushFront(20);
    list.pushFront(30);

    CHECK(list.size() == 3);
    CHECK(list.front() == 30);
    CHECK(list.back() == 10);
    CHECK(list[0] == 30);
    CHECK(list[1] == 20);
    CHECK(list[2] == 10);
}

TEST_CASE("DoubleList: pushBack() appends elements to the back") {
    fsd::core::DoubleList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);

    CHECK(list.size() == 3);
    CHECK(list.front() == 10);
    CHECK(list.back() == 30);
    CHECK(list[0] == 10);
    CHECK(list[1] == 20);
    CHECK(list[2] == 30);
}

TEST_CASE("DoubleList: insertAt() inserts element at arbitrary position") {
    fsd::core::DoubleList<int> list{1, 3, 4};

    list.insertAt(1, 2);

    CHECK(list.size() == 4);
    CHECK(list[0] == 1);
    CHECK(list[1] == 2);
    CHECK(list[2] == 3);
    CHECK(list[3] == 4);
}

TEST_CASE("DoubleList: removeAt() removes element and links remaining nodes") {
    fsd::core::DoubleList<int> list{1, 2, 3, 4};

    list.removeAt(1);

    CHECK(list.size() == 3);
    CHECK(list.front() == 1);
    CHECK(list.back() == 4);
    CHECK(list[0] == 1);
    CHECK(list[1] == 3);
    CHECK(list[2] == 4);
}

TEST_CASE("DoubleList: popFront() removes the first element") {
    fsd::core::DoubleList<int> list{1, 2, 3};

    list.popFront();

    CHECK(list.size() == 2);
    CHECK(list.front() == 2);
    CHECK(list.back() == 3);
    CHECK(list[0] == 2);
    CHECK(list[1] == 3);
}

TEST_CASE("DoubleList: popBack() removes the last element") {
    fsd::core::DoubleList<int> list{1, 2, 3};

    list.popBack();

    CHECK(list.size() == 2);
    CHECK(list.front() == 1);
    CHECK(list.back() == 2);
    CHECK(list[0] == 1);
    CHECK(list[1] == 2);
}

TEST_CASE("DoubleList: at() throws on out-of-bounds access") {
    fsd::core::DoubleList<int> list{1, 2, 3};

    CHECK_NOTHROW(list.at(0));
    CHECK_NOTHROW(list.at(2));
    CHECK_THROWS_AS(list.at(3), std::out_of_range);
}

TEST_CASE("DoubleList: update() replaces value at given index") {
    fsd::core::DoubleList<int> list{1, 2, 3};

    list.update(1, 99);

    CHECK(list.size() == 3);
    CHECK(list[0] == 1);
    CHECK(list[1] == 99);
    CHECK(list[2] == 3);
}

TEST_CASE("DoubleList: clear() removes all elements") {
    fsd::core::DoubleList<int> list{1, 2, 3, 4};

    list.clear();

    CHECK(list.size() == 0);
    CHECK(list.empty());
    CHECK(list.begin() == list.end());
    CHECK(list.rbegin() == list.rend());
}

TEST_CASE("DoubleList: copy construction creates equal independent copy") {
    fsd::core::DoubleList<int> original{1, 2, 3};
    fsd::core::DoubleList<int> copy(original);

    CHECK(copy == original);

    copy.update(1, 99);

    CHECK(copy[1] == 99);
    CHECK(original[1] == 2);
    CHECK(copy != original);
}

TEST_CASE("DoubleList: move construction transfers contents") {
    fsd::core::DoubleList<int> original{1, 2, 3};

    fsd::core::DoubleList<int> moved(std::move(original));

    CHECK(moved.size() == 3);
    CHECK(moved.front() == 1);
    CHECK(moved.back() == 3);
    CHECK(moved[0] == 1);
    CHECK(moved[1] == 2);
    CHECK(moved[2] == 3);

    CHECK(original.size() == 0);
    CHECK(original.empty());
}

TEST_CASE("DoubleList: find() returns iterator to matching element or end()") {
    fsd::core::DoubleList<int> list{5, 10, 15};

    auto it = list.find(10);
    auto missing = list.find(99);

    CHECK(it != list.end());
    CHECK(*it == 10);
    CHECK(missing == list.end());
}

TEST_CASE("DoubleList: iterators traverse elements in forward order") {
    fsd::core::DoubleList<int> list{1, 2, 3, 4};

    int expected = 1;
    for (auto it = list.begin(); it != list.end(); ++it) {
        CHECK(*it == expected);
        ++expected;
    }
}

TEST_CASE("DoubleList: reverse iterators traverse elements in reverse order") {
    fsd::core::DoubleList<int> list{1, 2, 3, 4};

    int expected = 4;
    for (auto it = list.rbegin(); it != list.rend(); ++it) {
        CHECK(*it == expected);
        --expected;
    }
}

TEST_CASE("DoubleList: comparison operators are lexicographical") {
    fsd::core::DoubleList<int> a{1, 2, 3};
    fsd::core::DoubleList<int> b{1, 2, 3};
    fsd::core::DoubleList<int> c{1, 2, 4};

    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
    CHECK(c > a);
    CHECK(a <= b);
    CHECK(c >= a);
}