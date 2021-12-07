cmake_minimum_required(VERSION 3.21)
project(matmult)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(TBB REQUIRED)

set(SOURCES main.cpp matrix.cpp matrix.hpp)

add_executable(matmult ${SOURCES})
target_compile_definitions(matmult PRIVATE SEQUENTIAL)

add_executable(matmult-pfor ${SOURCES})
target_compile_definitions(matmult-pfor PRIVATE PARALLELFOR)
target_link_libraries(matmult-pfor TBB::tbb)