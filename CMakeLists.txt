cmake_minimum_required(VERSION 3.0)
project(hashtables)

set(CMAKE_CXX_STANDARD 14)

option(PAUSE_ON_EXIT "Do not close console immideately before exit" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

add_executable(hashtables main.cpp hash_table.cpp hash_function.cpp hash_function.h cuckoo_hash_table.cpp open_hash_table.cpp)

if (PAUSE_ON_EXIT)
	target_compile_definitions(hashtables PUBLIC PAUSE_ON_EXIT)
endif(PAUSE_ON_EXIT)