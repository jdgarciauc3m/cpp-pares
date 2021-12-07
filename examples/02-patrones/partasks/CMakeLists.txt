cmake_minimum_required(VERSION 3.21)
project(partasks)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(TBB REQUIRED)
add_executable(partasks main.cpp)
target_link_libraries(partasks TBB::tbb)
