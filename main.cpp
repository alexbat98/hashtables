#include <iostream>

#include "hash_table.h"
#include "cuckoo_hash_table.h"
#include "open_hash_table.h"

#include <chrono>
#include <string>

int main() {

//    hash_table<int, std::string> ht;
//
//    std::string str = "User";
//    std::string str2 = "Another user";
//
//    ht.add(3, str);
//    ht.add(34569, str2);
//
//    std::string res = ht.get(3);
//
//    std::cout << res << std::endl;
//
//    ht.remove(3);
//
//    res = ht.get(34569);
//
//    std::cout << res << std::endl;
//
//    bool has = ht.has_key(3);
//
//    std::cout << has << std::endl;
//
//
//    long long seed = std::chrono::system_clock::now().time_since_epoch().count();
//    std::minstd_rand0 engine(static_cast<unsigned int>(seed));
//    std::uniform_int_distribution<int > distribution(1, 50000);
//
//    auto start = std::chrono::system_clock::now();
//    for (int i = 0; i < 1000000; i++) {
//        int key = distribution(engine);
//        std::string value = "value";
//
//        ht.add(key, value);
//    }
//    auto end = std::chrono::system_clock::now();
//
//    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << std::endl;
//
//    cuckoo_hash_table<int, std::string> cth(500);
//    cth.add(0, str);
//    cth.add(1, str);
//    cth.add(3, str);
//    cth.add(5, str);
//
//    std::cout << cth.has_key(0) << std::endl;
//    std::cout << cth.get(0) << std::endl;
//
//    open_hash_table<int, std::string> oth(500);
//    oth.add(0, str);
//    oth.add(1, str);
//    oth.add(3, str);
//    oth.add(5, str);
//
//    std::cout << oth.has_key(0) << std::endl;
//    std::cout << oth.get(0) << std::endl;

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distribution10k(0, 10000);
    std::uniform_int_distribution<uint64_t> distribution5b(0, 5000000000);

    std::cout << "Type;int_add;int_get;uint64_add;uint64_get" << std::endl;

    // 10k elements of type int, range 0..10k-1

    std::cout << "chain method;";
    hash_table<int, int> hashTable_int(10000);

    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        int key = distribution10k(generator);
        int value = distribution10k(generator);

        hashTable_int.add(key, value);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        int key = distribution10k(generator);
        hashTable_int.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";


    // 10k elements of type uint64_t, range 0..5b

    hash_table<uint64_t , uint64_t> hashTable_uint64(10000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        uint64_t key = distribution5b(generator);
        uint64_t value = distribution5b(generator);

        hashTable_uint64.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        uint64_t key = distribution5b(generator);
        hashTable_uint64.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;

    // 10k ints [0..10k]
//    cuckoo_hash_table<int, int> cuckooHashTable_int(20000);
//
//    start = std::chrono::steady_clock::now();
//    for (size_t i = 0; i < 10000; i++) {
//        int key = distribution10k(generator);
//        int value = distribution10k(generator);
//
//        cuckooHashTable_int.add(key, value);
//    }
//    end = std::chrono::steady_clock::now();
//    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";
//
//    start = std::chrono::steady_clock::now();
//    for (size_t i = 0; i < 10000; i++) {
//        int key = distribution10k(generator);
//        cuckooHashTable_int.has_key(key);
//    }
//    end = std::chrono::steady_clock::now();
//    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";



    // 10k of ints

    std::cout << "open addressing;";

    open_hash_table<int, int> openHashTable_int(10000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        int key = distribution10k(generator);
        int value = distribution10k(generator);

        openHashTable_int.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        int key = distribution10k(generator);
        openHashTable_int.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";


    // 10k of uint64
    open_hash_table<uint64_t , uint64_t > openHashTable_uint(10000);

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        uint64_t key = distribution5b(generator);
        uint64_t value = distribution5b(generator);

        openHashTable_uint.add(key, value);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

    start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        uint64_t key = distribution5b(generator);
        openHashTable_uint.has_key(key);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;


#ifdef PAUSE_ON_EXIT
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
#endif

    return 0;
}