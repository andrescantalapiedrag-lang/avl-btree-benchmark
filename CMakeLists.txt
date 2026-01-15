cmake_minimum_required(VERSION 3.16)
project(avl_btree_benchmark CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(avl_btree_benchmark
    src/main.cpp
    src/avl/AVLTree.cpp
    src/btree/BTree.cpp
)


target_include_directories(avl_btree_benchmark PRIVATE include)

if (MSVC)
    target_compile_options(avl_btree_benchmark PRIVATE /W4)
else()
    target_compile_options(avl_btree_benchmark PRIVATE -O2 -Wall -Wextra -Wpedantic)
endif()
