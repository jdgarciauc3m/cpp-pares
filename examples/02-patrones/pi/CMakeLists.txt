cmake_minimum_required(VERSION 3.21)
project(pi)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(TBB REQUIRED)
find_package(OpenMP REQUIRED)
add_executable(pi main.cpp)
target_link_libraries(pi TBB::tbb OpenMP::OpenMP_CXX)
