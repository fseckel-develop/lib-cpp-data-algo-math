// Created by Franz Seckel on 18.03.2026.
#include <doctest.h>
#include <fsd/core/binary_tree.hpp>

TEST_CASE("BinaryTree: default construction creates empty tree") {
    fsd::core::BinaryTree<int> tree;

    CHECK(tree.size() == 0);
    CHECK(tree.empty());
    CHECK(tree.height() == -1);
    CHECK(tree.begin() == tree.end());
}

TEST_CASE("BinaryTree: insert() adds first element as root") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(10);

    CHECK(tree.size() == 1);
    CHECK_FALSE(tree.empty());
    CHECK(tree.height() == 0);
    CHECK(tree.search(10));
}

TEST_CASE("BinaryTree: insert() fills tree in level-order") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    auto level = tree.levelOrder();

    REQUIRE(level.size() == 5);
    CHECK(level[0] == 1);
    CHECK(level[1] == 2);
    CHECK(level[2] == 3);
    CHECK(level[3] == 4);
    CHECK(level[4] == 5);

    CHECK(tree.height() == 2);
}

TEST_CASE("BinaryTree: search() returns true for inserted values and false otherwise") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    CHECK(tree.search(10));
    CHECK(tree.search(20));
    CHECK(tree.search(30));
    CHECK_FALSE(tree.search(99));
}

TEST_CASE("BinaryTree: preorder() returns preorder traversal") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    auto traversal = tree.preorder();

    REQUIRE(traversal.size() == 5);
    CHECK(traversal[0] == 1);
    CHECK(traversal[1] == 2);
    CHECK(traversal[2] == 4);
    CHECK(traversal[3] == 5);
    CHECK(traversal[4] == 3);
}

TEST_CASE("BinaryTree: inorder() returns inorder traversal") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    auto traversal = tree.inorder();

    REQUIRE(traversal.size() == 5);
    CHECK(traversal[0] == 4);
    CHECK(traversal[1] == 2);
    CHECK(traversal[2] == 5);
    CHECK(traversal[3] == 1);
    CHECK(traversal[4] == 3);
}

TEST_CASE("BinaryTree: postorder() returns postorder traversal") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    auto traversal = tree.postorder();

    REQUIRE(traversal.size() == 5);
    CHECK(traversal[0] == 4);
    CHECK(traversal[1] == 5);
    CHECK(traversal[2] == 2);
    CHECK(traversal[3] == 3);
    CHECK(traversal[4] == 1);
}

TEST_CASE("BinaryTree: levelOrder() returns breadth-first traversal") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    auto traversal = tree.levelOrder();

    REQUIRE(traversal.size() == 5);
    CHECK(traversal[0] == 1);
    CHECK(traversal[1] == 2);
    CHECK(traversal[2] == 3);
    CHECK(traversal[3] == 4);
    CHECK(traversal[4] == 5);
}

TEST_CASE("BinaryTree: remove() removes existing value from tree") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);

    tree.remove(2);

    CHECK(tree.size() == 3);
    CHECK_FALSE(tree.search(2));
}

TEST_CASE("BinaryTree: remove() does nothing for missing value") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    tree.remove(99);

    CHECK(tree.size() == 3);
    CHECK(tree.search(1));
    CHECK(tree.search(2));
    CHECK(tree.search(3));
}

TEST_CASE("BinaryTree: clear() removes all elements") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    tree.clear();

    CHECK(tree.size() == 0);
    CHECK(tree.empty());
    CHECK(tree.height() == -1);
    CHECK(tree.begin() == tree.end());
}

TEST_CASE("BinaryTree: copy construction creates equal independent tree structure") {
    fsd::core::BinaryTree<int> original;

    original.insert(1);
    original.insert(2);
    original.insert(3);

    fsd::core::BinaryTree<int> copy(original);

    auto originalLevel = original.levelOrder();
    auto copyLevel = copy.levelOrder();

    REQUIRE(copyLevel.size() == originalLevel.size());
    for (std::size_t i = 0; i < originalLevel.size(); ++i) {
        CHECK(copyLevel[i] == originalLevel[i]);
    }

    copy.insert(4);

    CHECK(copy.size() == 4);
    CHECK(original.size() == 3);
}

TEST_CASE("BinaryTree: move construction transfers contents") {
    fsd::core::BinaryTree<int> original;

    original.insert(1);
    original.insert(2);
    original.insert(3);

    fsd::core::BinaryTree<int> moved(std::move(original));

    CHECK(moved.size() == 3);
    CHECK(moved.search(1));
    CHECK(moved.search(2));
    CHECK(moved.search(3));

    CHECK(original.empty());
}

TEST_CASE("BinaryTree: iterators traverse tree in inorder sequence") {
    fsd::core::BinaryTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    int expected[] = {4, 2, 5, 1, 3};
    int index = 0;

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        REQUIRE(index < 5);
        CHECK(*it == expected[index]);
        ++index;
    }

    CHECK(index == 5);
}