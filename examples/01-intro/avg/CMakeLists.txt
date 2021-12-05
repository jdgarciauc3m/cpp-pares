cmake_minimum_required(VERSION 3.20)
project(avg)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(TBB REQUIRED)

add_executable(avg main.cpp)
target_link_libraries(avg TBB::tbb)
