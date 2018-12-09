#include <iostream>

#include "hash_table.h"
#include "cuckoo_hash_table.h"
#include "open_hash_table.h"
#include "k_independent_hash_function.h"

#include <chrono>
#include <string>

std::string gen_random(const int len) {
    std::string res;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        res.push_back(alphanum[std::rand() % (sizeof(alphanum) - 1)]);
    }

    return res;
}

int main() {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distribution10k(0, 15000000);
    std::uniform_int_distribution<uint64_t> distribution5b(0, 5000000000);

    std::cout << "Type;int_add;int_get;uint64_add;uint64_get" << std::endl;

    // 10k elements of type int, range 0..10k-1

    std::cout << "chain method;";
    hash_table<int, int> hashTable_int(100000);

    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        int key = distribution10k(generator);
        int value = distribution10k(generator);

        hashTable_int.add(key, value);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        int key = distribution10k(generator);
        hashTable_int.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";


    // 10k elements of type uint64_t, range 0..5b

    hash_table<uint64_t , uint64_t> hashTable_uint64(100000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        uint64_t key = distribution5b(generator);
        uint64_t value = distribution5b(generator);

        hashTable_uint64.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        uint64_t key = distribution5b(generator);
        hashTable_uint64.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    hash_table<std::string, uint64_t> hashTable_string(100000, std::make_shared<k_independent_hash_function<std::string, 5>>());

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        std::string key = gen_random(15);
        uint64_t value = distribution5b(generator);

        hashTable_string.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        std::string key = gen_random(15);
        hashTable_string.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;

    // 10k ints [0..10k]
    std::cout << "cuckoo addressing;";
    cuckoo_hash_table<int, int> cuckooHashTable_int(100000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        int key = distribution10k(generator);
        int value = distribution10k(generator);

        cuckooHashTable_int.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        int key = distribution10k(generator);
        cuckooHashTable_int.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    cuckoo_hash_table<uint64_t, uint64_t> cuckooHashTable_uint(100000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        uint64_t key = distribution5b(generator);
        uint64_t value = distribution5b(generator);

        cuckooHashTable_uint.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        uint64_t key = distribution5b(generator);
        cuckooHashTable_uint.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    // strings
    cuckoo_hash_table<std::string, uint64_t> cuckooHashTable_string(100000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        std::string key = gen_random(15);
        uint64_t value = distribution5b(generator);

        cuckooHashTable_string.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        std::string key = gen_random(15);
        cuckooHashTable_string.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    std::cout << std::endl;

    // 10k of ints

    std::cout << "open addressing;";

    open_hash_table<int, int> openHashTable_int(100000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        int key = distribution10k(generator);
        int value = distribution10k(generator);

        openHashTable_int.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        int key = distribution10k(generator);
        openHashTable_int.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";


    // 10k of uint64
    open_hash_table<uint64_t , uint64_t > openHashTable_uint(100000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        uint64_t key = distribution5b(generator);
        uint64_t value = distribution5b(generator);

        openHashTable_uint.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        uint64_t key = distribution5b(generator);
        openHashTable_uint.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    // strings

    open_hash_table<std::string , uint64_t > openHashTable_string(100000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        std::string key = gen_random(15);
        uint64_t value = distribution5b(generator);

        openHashTable_string.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 100000; i++) {
        std::string key = gen_random(15);
        openHashTable_string.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";
    std::cout << std::endl;


//#ifdef PAUSE_ON_EXIT
//	std::cout << "Press any key to continue..." << std::endl;
//	std::cin.get();
//#endif

    return 0;
}