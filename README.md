# 🚀 fsd-cpp

![Language](https://img.shields.io/badge/Language-C%2B%2B%2020-orange)
![CMake](https://img.shields.io/badge/Build-CMake%203.25+-blue)
![Tests](https://img.shields.io/badge/Tests-doctest-green)
![Status](https://img.shields.io/badge/Status-Active%20Development-yellow)
![Platform](https://img.shields.io/badge/Platform-cross--platform-lightgrey)

A modern C++ library for data structures, algorithms, mathematical types and operations.  
`fsd-cpp` unifies two main domains:

- **Core** — classical data structures and general-purpose algorithms  
- **Math** — algebraic, analytic, and geometric types and operations  

The project emphasizes **clean design**, **modular architecture**, and **modern C++ practices**, and is intended as a foundation for reusable components and long-term library development.

---
## 🚧 Current Status

This project is under active refactoring and continuous development.

---
## ⚙️ Build and Install

Requirements:
- CMake ≥ 3.25
- C++20 compatible compiler (Clang, GCC, MSVC)

Configure:
```shell
cmake -S . -B build
```

Build:
```shell
cmake --build build
```

Install:
```shell
cmake --install build --prefix install
```

This installs:
- public headers under include/
- CMake package configuration for downstream usage

Once done, the library can be integrated using CMake:
```cmake
find_package(fsd REQUIRED)
target_link_libraries(your_target PRIVATE fsd::fsd)
```

---
## 🗃️ Project Structure

```text
.
├── CMakeLists.txt
├── README.md
├── LICENSE
├── include/
│   └── fsd/
│       ├── core/
│       │   ├── Array
│       │   ├── BinaryTree
│       │   ├── Deque
│       │   ├── DoubleList
│       │   ├── Queue
│       │   ├── SearchTree
│       │   ├── SingleList
│       │   ├── Stack
│       │   └── Vector
│       └── math/
│           ├── algebra/
│           │   ├── Vector
│           │   ├── Matrix
│           │   └── LinearTransform
│           ├── analysis/
│           └── geometry/
│               └── Point
├── examples/
├── tests/
└── cmake/
```

---
## 🧩 Modules

### core/

Fundamental data structures and algorithmic building blocks.

Includes:
- array, vector
- stack, queue, deque
- linked lists
- binary and search trees

Planned extensions:
- sorting and searching algorithms
- graph data structures and algorithms
- traversal and utility abstractions

### math/algebra/

Linear algebra and related mathematical structures.

Includes:
- vector
- matrix
- linear transform

Planned extensions:
- operations (dot product, cross product, norms, determinants, inverses, ...)
- algorithms (Gaussian elimination, matrix decomposition, eigenvalue methods, solving linear systems, ...)
- transforms (rotation, reflection, scaling, shear, projection, ...)
- supporting abstractions (basis, affine transform, change of basis, ...)

Design:
- separation into storage, operations, algorithms, and aliases
- modular, composable headers

### math/geometry/

Geometric primitives and operations.

Includes:
- point

Planned extensions:
- primitives (line, segment, ray, plane, …)
- shapes (triangle, rectangle, circle, polygon, box, sphere, …)
- algorithms (intersection, containment, distance, area, centroid, convex hull, …)
- transforms (translation, rotation, scaling, projection)

Architecturally, the module distinguishes between geometric primitives, higher-level shapes, algorithms, and transformations.

### math/analysis/

Numerical and functional analysis tools for evaluating and transforming functions.

Planned extensions:
- differentiation (numerical and symbolic)
- integration (trapezoidal, Simpson, adaptive methods)
- series expansions (Taylor series)
- transforms (Fourier transform and related methods)

The module is intended to combine a high-level representation of mathematical functions with efficient numerical algorithms.

Initial focus:
- numerical differentiation and integration
- simple function abstractions
- extensible design for future symbolic capabilities

---
## 🏗 Architecture

The library is structured by domain, not as a flat collection of headers.

Design principles:
- Clear separation between core, algebra, and geometry
- Small, composable components (storage / operations / algorithms / aliases)
- Modern C++ (C++20) with emphasis on clarity and type safety
- Incremental, long-term evolution

A key design goal is to separate concepts across domains — for example, a mathematical vector and a dynamic array are treated as distinct abstractions.

---
## 🧪 Testing

The project uses a lightweight test setup based on **`doctest`**.

Build tests:
```shell
cmake -S . -B build -DFSD_BUILD_TESTS=ON
cmake --build build
```

Run tests via `ctest`:
```shell
ctest --test-dir build
# OR
ctest --test-dir build -V    # verbose output (shows individual test cases)
```

Run tests via `fsd-tests` executable:
```shell
./build/tests/fsd-tests
# OR
./build/tests/fsd-tests -s                  # show all test cases
# OR
./build/tests/fsd-tests -tc="Vector*"       # run only tests matching "Vector"
# OR
./build/tests/fsd-tests --list-test-cases   # list all test cases
```

Notes:
- Tests are located in the `tests/` directory
- Test cases are grouped by module (e.g. `core/`, `math/`)
- Doctest supports filtering, tagging, and fine-grained execution

---
## 🎯 Purpose

This repository is intended to be:
- a personal reference implementation library
- a place to practice library and API design in C++
- a foundation for future algorithmic and mathematical extensions
- a unified home for reusable core and mathematical components

---
## 🗺 Roadmap

Planned additions include:
- namespace cleanup and API consolidation
- consistent naming across modules
- classic sorting and searching algorithms
- graph structures and graph algorithms
- more linear algebra operations
- analysis and numerical methods
- more geometry primitives and utilities
