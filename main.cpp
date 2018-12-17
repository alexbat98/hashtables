#include <iostream>

#include "hash_table.h"
#include "cuckoo_hash_table.h"
#include "open_hash_table.h"
//#include "k_independent_hash_function.h"

#include <chrono>
#include <string>
#include <memory>

std::string &gen_random(const int len) {

    char *str = new char[len+1];

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        str[i] = (alphanum[std::rand() % (sizeof(alphanum) - 1)]);
    }
    str[len] = '\0';

    auto *res = new std::string(str);
    return *res;
}

int main() {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distribution10k(0, 990000000);
    std::uniform_int_distribution<uint64_t > distribution5b(0, 50000000000);
//    std::binomial_distribution<int> distribution10k(70000000, 40000000);
//    std::binomial_distribution<double> distribution5b(90000000, 80000000);

    uint64_t count[] = {50000, 100000, 500000, 1000000};

    std::vector<int> dataInt;
    std::vector<uint64_t> dataInt64;
    std::vector<std::string> dataString;

    hash_table<int, int> hashTable_int;
    cuckoo_hash_table<int, int> cuckooHashTable_int;
    open_hash_table<int, int> openHashTable_int;

    hash_table<uint64_t, uint64_t> hashTable_uint64;
    cuckoo_hash_table<uint64_t, uint64_t> cuckooHashTable_uint;
    open_hash_table<uint64_t, uint64_t> openHashTable_uint;

    hash_table<std::string, uint64_t> hashTable_string;
    cuckoo_hash_table<std::string, uint64_t> cuckooHashTable_string;
    open_hash_table<std::string, uint64_t> openHashTable_string;

    for (int mult = 1; mult < 3; mult++) {
        for (unsigned long long k : count) {

            dataInt.clear();
            dataInt64.clear();
            dataString.clear();

            dataInt.reserve(k);
            dataInt64.reserve(k);
            dataString.reserve(k);


            // INTEGER
            for (int i = 0; i < k; i++) {
                dataInt[i] = static_cast<int>(distribution10k(generator));
            }

            hashTable_int = hash_table<int, int>(mult * k);
            cuckooHashTable_int = cuckoo_hash_table<int, int>(mult * k);
            openHashTable_int = open_hash_table<int, int>(mult * k);

            // add
            auto start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                int value = static_cast<int>(distribution10k(generator));

                hashTable_int.add(key, value);
            }
            auto end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                hashTable_int.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                hashTable_int.remove(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // add
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                int value = static_cast<int>(distribution10k(generator));

                cuckooHashTable_int.add(key, value);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                cuckooHashTable_int.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                cuckooHashTable_int.remove(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // add
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                int value = static_cast<int>(distribution10k(generator));

                openHashTable_int.add(key, value);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                openHashTable_int.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                int key = dataInt[i];
                openHashTable_int.remove(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";



            // INT64

            for (int i = 0; i < k; i++) {
                dataInt64[i] = static_cast<uint64_t>(distribution5b(generator));
            }

            hashTable_uint64 = hash_table<uint64_t, uint64_t>(mult * k);
            cuckooHashTable_uint = cuckoo_hash_table<uint64_t, uint64_t>(mult * k);
            openHashTable_uint = open_hash_table<uint64_t, uint64_t>(mult * k);

            // add
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                uint64_t value = static_cast<uint64_t>(distribution5b(generator));

                hashTable_uint64.add(key, value);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                hashTable_uint64.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                hashTable_uint64.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // add
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                uint64_t value = static_cast<uint64_t>(distribution5b(generator));

                cuckooHashTable_uint.add(key, value);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                cuckooHashTable_uint.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                cuckooHashTable_uint.remove(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";


            // add
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                uint64_t value = static_cast<uint64_t>(distribution5b(generator));

                openHashTable_uint.add(key, value);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                openHashTable_uint.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                uint64_t key = dataInt64[i];
                openHashTable_uint.remove(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";


            // STRINGS

            for (int i = 0; i < k; i++) {
                dataString.insert(dataString.begin() + i, gen_random(8));
            }

            std::vector<bool> reading_base;
            reading_base.reserve(k);

            hashTable_string
                = hash_table<std::string, uint64_t> (mult * k,
                    std::make_shared<k_independent_hash_function<std::string, 5>>());
            cuckooHashTable_string = cuckoo_hash_table<std::string, uint64_t> (mult * k);
            openHashTable_string = open_hash_table<std::string, uint64_t>(mult * k);

            // add
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                uint64_t value = static_cast<uint64_t>(distribution5b(generator));

                hashTable_string.add(key, value);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                reading_base[i] = hashTable_string.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                hashTable_string.remove(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";


            // add
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                uint64_t value = static_cast<uint64_t>(distribution5b(generator));

                cuckooHashTable_string.add(key, value);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                reading_base[i] = cuckooHashTable_string.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                cuckooHashTable_string.remove(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";


            // add
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                uint64_t value = static_cast<uint64_t>(distribution5b(generator));

                openHashTable_string.add(key, value);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // search
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                reading_base[i] = openHashTable_string.has_key(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            // remove
            start = std::chrono::steady_clock::now();
            for (size_t i = 0; i < k; i++) {
                std::string key = dataString[i];
                openHashTable_string.remove(key);
            }
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration<double, std::milli>(end - start).count() << ";";

            std::cout << std::endl;
        }
    }

//#ifdef PAUSE_ON_EXIT
//	std::cout << "Press any key to continue..." << std::endl;
//	std::cin.get();
//#endif

    return 0;
}