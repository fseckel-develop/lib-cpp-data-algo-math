cmake_minimum_required(VERSION 4.00)
project(cpp-core)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# setting header files
set(HEADER_FILES
        src/Array.h
)

# setting source files
set(SOURCE_FILES
        main.cpp
)

# adding main executable
add_executable(cpp-core
        main.cpp
        ${HEADER_FILES}
        ${SOURCE_FILES}
)
