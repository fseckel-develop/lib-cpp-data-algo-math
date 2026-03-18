// Created by Franz Seckel on 17.03.2026.
#include <doctest.h>
#include <fsd/core/queue.hpp>

TEST_CASE("Queue: default construction creates empty queue") {
    fsd::core::Queue<int> q;

    CHECK(q.size() == 0);
    CHECK(q.empty());
}

TEST_CASE("Queue: enqueue() adds elements to the back") {
    fsd::core::Queue<int> q;

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);

    CHECK(q.size() == 3);
    CHECK_FALSE(q.empty());
    CHECK(q.front() == 10);
}

TEST_CASE("Queue: dequeue() removes elements from the front") {
    fsd::core::Queue<int> q;

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);

    q.dequeue();

    CHECK(q.size() == 2);
    CHECK(q.front() == 20);
}

TEST_CASE("Queue: maintains FIFO order") {
    fsd::core::Queue<int> q;

    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    CHECK(q.front() == 1);
    q.dequeue();
    CHECK(q.front() == 2);
    q.dequeue();
    CHECK(q.front() == 3);
}

TEST_CASE("Queue: swap() exchanges contents") {
    fsd::core::Queue<int> a;
    fsd::core::Queue<int> b;

    a.enqueue(1);
    a.enqueue(2);

    b.enqueue(10);

    a.swap(b);

    CHECK(a.size() == 1);
    CHECK(a.front() == 10);

    CHECK(b.size() == 2);
    CHECK(b.front() == 1);
}