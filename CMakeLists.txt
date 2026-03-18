cmake_minimum_required(VERSION 3.25)
project(fsd-cpp
        VERSION 0.1.0
        DESCRIPTION "Modern C++ library for data structures, algorithms, mathematical types and operations"
        LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(FSD_BUILD_EXAMPLES "Build fsd-cpp examples" ON)
option(FSD_BUILD_TESTS "Build fsd-cpp tests" ON)

add_library(fsd INTERFACE)
add_library(fsd::fsd ALIAS fsd)

target_compile_features(fsd INTERFACE cxx_std_20)

target_include_directories(fsd
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE
        ${PROJECT_SOURCE_DIR}/external/doctest
)

if (MSVC)
    target_compile_options(fsd INTERFACE
            /W4
            /permissive-
            /we4265
            /we5204
            /wd4100
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(fsd INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wnon-virtual-dtor
            -Wreorder
            -Wmaybe-uninitialized
            -Wunused-variable
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(fsd INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wnon-virtual-dtor
            -Wreorder
            -Wsometimes-uninitialized
            -Wunused-variable
    )
endif()

if (FSD_BUILD_EXAMPLES)
    add_executable(fsd-example examples/main.cpp)
    target_link_libraries(fsd-example PRIVATE fsd::fsd)
endif()

if (FSD_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS fsd
        EXPORT fsdTargets
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT fsdTargets
        FILE fsdTargets.cmake
        NAMESPACE fsd::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fsd
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fsdConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/fsdConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fsd
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/fsdConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/fsdConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/fsdConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fsd
)