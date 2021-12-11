cmake_minimum_required(VERSION 3.21)
project(prefix_sum)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(TBB REQUIRED)

add_executable(prefix_sum main.cpp)
target_link_libraries(prefix_sum TBB::tbb)
