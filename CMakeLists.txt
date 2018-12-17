cmake_minimum_required(VERSION 3.0)
project(hashtables)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -m64)

option(PAUSE_ON_EXIT "Do not close console immideately before exit" ON)

add_executable(hashtables main.cpp hash_table.cpp hash_function.cpp cuckoo_hash_table.cpp open_hash_table.cpp k_independent_hash_function.cpp)

if (PAUSE_ON_EXIT)
	target_compile_definitions(hashtables PUBLIC PAUSE_ON_EXIT)
endif(PAUSE_ON_EXIT)