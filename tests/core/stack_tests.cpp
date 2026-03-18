// Created by Franz Seckel on 17.03.2026.
#include <doctest.h>
#include <fsd/core/stack.hpp>

TEST_CASE("Stack: default construction creates empty stack") {
    fsd::core::Stack<int> s;

    CHECK(s.size() == 0);
    CHECK(s.empty());
}

TEST_CASE("Stack: push() adds elements to the top") {
    fsd::core::Stack<int> s;

    s.push(10);
    s.push(20);
    s.push(30);

    CHECK(s.size() == 3);
    CHECK_FALSE(s.empty());
    CHECK(s.top() == 30);
}

TEST_CASE("Stack: pop() removes the top element") {
    fsd::core::Stack<int> s;

    s.push(10);
    s.push(20);
    s.push(30);

    s.pop();

    CHECK(s.size() == 2);
    CHECK(s.top() == 20);
}

TEST_CASE("Stack: maintains LIFO order") {
    fsd::core::Stack<int> s;

    s.push(1);
    s.push(2);
    s.push(3);

    CHECK(s.top() == 3);
    s.pop();
    CHECK(s.top() == 2);
    s.pop();
    CHECK(s.top() == 1);
}

TEST_CASE("Stack: swap() exchanges contents") {
    fsd::core::Stack<int> a;
    fsd::core::Stack<int> b;

    a.push(1);
    a.push(2);

    b.push(10);

    a.swap(b);

    CHECK(a.size() == 1);
    CHECK(a.top() == 10);

    CHECK(b.size() == 2);
    CHECK(b.top() == 2);
}