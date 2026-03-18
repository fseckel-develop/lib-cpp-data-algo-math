// Created by Franz Seckel on 17.03.2026.
#include <doctest.h>
#include <fsd/core/single_list.hpp>

TEST_CASE("SingleList: default construction creates empty list") {
    fsd::core::SingleList<int> list;

    CHECK(list.size() == 0);
    CHECK(list.empty());
    CHECK(list.begin() == list.end());
}

TEST_CASE("SingleList: initializer_list constructor preserves element order") {
    fsd::core::SingleList<int> list{1, 2, 3};

    CHECK(list.size() == 3);
    CHECK_FALSE(list.empty());
    CHECK(list.front() == 1);
    CHECK(list.back() == 3);
    CHECK(list[0] == 1);
    CHECK(list[1] == 2);
    CHECK(list[2] == 3);
}

TEST_CASE("SingleList: pushFront() prepends elements to the front") {
    fsd::core::SingleList<int> list;

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

TEST_CASE("SingleList: pushBack() appends elements to the back") {
    fsd::core::SingleList<int> list;

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

TEST_CASE("SingleList: insertAt() inserts element at arbitrary position") {
    fsd::core::SingleList<int> list{1, 3, 4};

    list.insertAt(1, 2);

    CHECK(list.size() == 4);
    CHECK(list[0] == 1);
    CHECK(list[1] == 2);
    CHECK(list[2] == 3);
    CHECK(list[3] == 4);
}

TEST_CASE("SingleList: removeAt() removes element and links remaining nodes") {
    fsd::core::SingleList<int> list{1, 2, 3, 4};

    list.removeAt(1);

    CHECK(list.size() == 3);
    CHECK(list.front() == 1);
    CHECK(list.back() == 4);
    CHECK(list[0] == 1);
    CHECK(list[1] == 3);
    CHECK(list[2] == 4);
}

TEST_CASE("SingleList: popFront() removes the first element") {
    fsd::core::SingleList<int> list{1, 2, 3};

    list.popFront();

    CHECK(list.size() == 2);
    CHECK(list.front() == 2);
    CHECK(list.back() == 3);
    CHECK(list[0] == 2);
    CHECK(list[1] == 3);
}

TEST_CASE("SingleList: popBack() removes the last element") {
    fsd::core::SingleList<int> list{1, 2, 3};

    list.popBack();

    CHECK(list.size() == 2);
    CHECK(list.front() == 1);
    CHECK(list.back() == 2);
    CHECK(list[0] == 1);
    CHECK(list[1] == 2);
}

TEST_CASE("SingleList: at() throws on out-of-bounds access") {
    fsd::core::SingleList<int> list{1, 2, 3};

    CHECK_NOTHROW(list.at(0));
    CHECK_NOTHROW(list.at(2));
    CHECK_THROWS_AS(list.at(3), std::out_of_range);
}

TEST_CASE("SingleList: update() replaces value at given index") {
    fsd::core::SingleList<int> list{1, 2, 3};

    list.update(1, 99);

    CHECK(list.size() == 3);
    CHECK(list[0] == 1);
    CHECK(list[1] == 99);
    CHECK(list[2] == 3);
}

TEST_CASE("SingleList: clear() removes all elements") {
    fsd::core::SingleList<int> list{1, 2, 3, 4};

    list.clear();

    CHECK(list.size() == 0);
    CHECK(list.empty());
    CHECK(list.begin() == list.end());
}

TEST_CASE("SingleList: copy construction creates equal independent copy") {
    fsd::core::SingleList<int> original{1, 2, 3};
    fsd::core::SingleList<int> copy(original);

    CHECK(copy == original);

    copy.update(1, 99);

    CHECK(copy[1] == 99);
    CHECK(original[1] == 2);
    CHECK(copy != original);
}

TEST_CASE("SingleList: move construction transfers contents") {
    fsd::core::SingleList<int> original{1, 2, 3};

    fsd::core::SingleList<int> moved(std::move(original));

    CHECK(moved.size() == 3);
    CHECK(moved.front() == 1);
    CHECK(moved.back() == 3);
    CHECK(moved[0] == 1);
    CHECK(moved[1] == 2);
    CHECK(moved[2] == 3);

    CHECK(original.size() == 0);
    CHECK(original.empty());
}

TEST_CASE("SingleList: find() returns iterator to matching element or end()") {
    fsd::core::SingleList<int> list{5, 10, 15};

    auto it = list.find(10);
    auto missing = list.find(99);

    CHECK(it != list.end());
    CHECK(*it == 10);
    CHECK(missing == list.end());
}

TEST_CASE("SingleList: iterators traverse elements in forward order") {
    fsd::core::SingleList<int> list{1, 2, 3, 4};

    int expected = 1;
    for (auto it = list.begin(); it != list.end(); ++it) {
        CHECK(*it == expected);
        ++expected;
    }
}

TEST_CASE("SingleList: comparison operators are lexicographical") {
    fsd::core::SingleList<int> a{1, 2, 3};
    fsd::core::SingleList<int> b{1, 2, 3};
    fsd::core::SingleList<int> c{1, 2, 4};

    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
    CHECK(c > a);
    CHECK(a <= b);
    CHECK(c >= a);
}