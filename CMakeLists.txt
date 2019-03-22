cmake_minimum_required(VERSION 3.10)
project(big_iterators_benchmark)

set(CMAKE_CXX_STANDARD 17)

include_directories(third_party/range-v3/include)

add_executable(big_iterators_benchmark src/main.cpp)