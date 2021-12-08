cmake_minimum_required(VERSION 3.21)
project(maxvalue)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

find_package(TBB REQUIRED)

add_executable(maxvalue main.cpp)
target_link_libraries(maxvalue TBB::tbb)
