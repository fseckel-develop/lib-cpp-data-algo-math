// Created by Franz Seckel on 08.10.25.
#include <iostream>
#include <ostream>

#include "src/Array.h"

int main() {
    Array<int, 10> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << arr.size() << std::endl;
}
