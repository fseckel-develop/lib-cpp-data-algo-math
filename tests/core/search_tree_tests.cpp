// Created by Franz Seckel on 18.03.2026.
#include <doctest.h>
#include <fsd/core/search_tree.hpp>

TEST_CASE("SearchTree: default construction creates empty tree") {
    fsd::core::SearchTree<int> tree;

    CHECK(tree.size() == 0);
    CHECK(tree.empty());
    CHECK(tree.height() == -1);
    CHECK(tree.begin() == tree.end());
}

TEST_CASE("SearchTree: insert() preserves binary search tree ordering") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);

    auto traversal = tree.inorder();

    REQUIRE(traversal.size() == 7);
    CHECK(traversal[0] == 1);
    CHECK(traversal[1] == 2);
    CHECK(traversal[2] == 3);
    CHECK(traversal[3] == 4);
    CHECK(traversal[4] == 5);
    CHECK(traversal[5] == 6);
    CHECK(traversal[6] == 7);
}

TEST_CASE("SearchTree: insert() ignores duplicate values") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(4);
    tree.insert(2);
    tree.insert(6);

    auto traversal = tree.inorder();

    REQUIRE(traversal.size() == 3);
    CHECK(traversal[0] == 2);
    CHECK(traversal[1] == 4);
    CHECK(traversal[2] == 6);
}

TEST_CASE("SearchTree: search() returns true for inserted values and false otherwise") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);

    CHECK(tree.search(4));
    CHECK(tree.search(2));
    CHECK(tree.search(6));
    CHECK_FALSE(tree.search(99));
}

TEST_CASE("SearchTree: min() returns smallest value") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);

    CHECK(tree.min() == 1);
}

TEST_CASE("SearchTree: max() returns largest value") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(5);
    tree.insert(7);

    CHECK(tree.max() == 7);
}

TEST_CASE("SearchTree: min() and max() throw on empty tree") {
    fsd::core::SearchTree<int> tree;

    CHECK_THROWS_AS(tree.min(), std::invalid_argument);
    CHECK_THROWS_AS(tree.max(), std::invalid_argument);
}

TEST_CASE("SearchTree: find() returns iterator to matching value or end()") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);

    auto found = tree.find(2);
    auto missing = tree.find(99);

    CHECK(found != tree.end());
    CHECK(*found == 2);
    CHECK(missing == tree.end());
}

TEST_CASE("SearchTree: lowerBound() returns first element not less than value") {
    fsd::core::SearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(18);

    auto it1 = tree.lowerBound(7);
    auto it2 = tree.lowerBound(8);
    auto it3 = tree.lowerBound(20);

    CHECK(it1 != tree.end());
    CHECK(*it1 == 7);

    CHECK(it2 != tree.end());
    CHECK(*it2 == 10);

    CHECK(it3 == tree.end());
}

TEST_CASE("SearchTree: upperBound() returns first element greater than value") {
    fsd::core::SearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(18);

    auto it1 = tree.upperBound(7);
    auto it2 = tree.upperBound(8);
    auto it3 = tree.upperBound(18);

    CHECK(it1 != tree.end());
    CHECK(*it1 == 10);

    CHECK(it2 != tree.end());
    CHECK(*it2 == 10);

    CHECK(it3 == tree.end());
}

TEST_CASE("SearchTree: remove() deletes leaf node") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);

    tree.remove(1);

    CHECK_FALSE(tree.search(1));
    CHECK(tree.size() == 3);

    auto traversal = tree.inorder();
    REQUIRE(traversal.size() == 3);
    CHECK(traversal[0] == 2);
    CHECK(traversal[1] == 4);
    CHECK(traversal[2] == 6);
}

TEST_CASE("SearchTree: remove() deletes node with one child") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);

    tree.remove(2);

    CHECK_FALSE(tree.search(2));
    CHECK(tree.size() == 3);

    auto traversal = tree.inorder();
    REQUIRE(traversal.size() == 3);
    CHECK(traversal[0] == 1);
    CHECK(traversal[1] == 4);
    CHECK(traversal[2] == 6);
}

TEST_CASE("SearchTree: remove() deletes node with two children") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);

    tree.remove(4);

    CHECK_FALSE(tree.search(4));
    CHECK(tree.size() == 6);

    auto traversal = tree.inorder();
    REQUIRE(traversal.size() == 6);
    CHECK(traversal[0] == 1);
    CHECK(traversal[1] == 2);
    CHECK(traversal[2] == 3);
    CHECK(traversal[3] == 5);
    CHECK(traversal[4] == 6);
    CHECK(traversal[5] == 7);
}

TEST_CASE("SearchTree: remove() does nothing for missing value") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);

    tree.remove(99);

    CHECK(tree.size() == 3);
    CHECK(tree.search(4));
    CHECK(tree.search(2));
    CHECK(tree.search(6));
}

TEST_CASE("SearchTree: iterators traverse values in inorder sequence") {
    fsd::core::SearchTree<int> tree;

    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);

    int expected[] = {1, 2, 3, 4, 5, 6, 7};
    int index = 0;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        REQUIRE(index < 7);
        CHECK(*it == expected[index]);
        ++index;
    }

    CHECK(index == 7);
}

TEST_CASE("SearchTree: copy construction creates equal independent tree") {
    fsd::core::SearchTree<int> original;

    original.insert(4);
    original.insert(2);
    original.insert(6);

    fsd::core::SearchTree<int> copy(original);

    auto originalTraversal = original.inorder();
    auto copyTraversal = copy.inorder();

    REQUIRE(copyTraversal.size() == originalTraversal.size());
    for (std::size_t i = 0; i < originalTraversal.size(); ++i) {
        CHECK(copyTraversal[i] == originalTraversal[i]);
    }

    copy.insert(1);

    CHECK(copy.size() == 4);
    CHECK(original.size() == 3);
}

TEST_CASE("SearchTree: move construction transfers contents") {
    fsd::core::SearchTree<int> original;

    original.insert(4);
    original.insert(2);
    original.insert(6);

    fsd::core::SearchTree<int> moved(std::move(original));

    CHECK(moved.size() == 3);
    CHECK(moved.search(4));
    CHECK(moved.search(2));
    CHECK(moved.search(6));

    CHECK(original.empty());
}