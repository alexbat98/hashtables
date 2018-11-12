cmake_minimum_required(VERSION 3.0)
project(hashtables)

set(CMAKE_CXX_STANDARD 11)

add_executable(hashtables main.cpp hash_table.cpp prime_number_generator.cpp hash_function.cpp hash_function.h generic_hash_function.cpp generic_hash_function.h)