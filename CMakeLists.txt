cmake_minimum_required(VERSION 3.0)
project(hashtables)

set(CMAKE_CXX_STANDARD 14)

option(PAUSE_ON_EXIT "Do not close console immideately before exit" ON)

add_executable(hashtables main.cpp hash_table.cpp prime_number_generator.cpp hash_function.cpp hash_function.h generic_hash_function.cpp generic_hash_function.h wiki_hash_function.cpp cuckoo_hash_table.cpp open_hash_table.cpp open_hash_table.h k_independent_hash_function.cpp k_independent_hash_function.h)

if (PAUSE_ON_EXIT)
	target_compile_definitions(hashtables PUBLIC PAUSE_ON_EXIT)
endif(PAUSE_ON_EXIT)