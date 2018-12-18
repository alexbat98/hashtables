#ifndef HASHTABLES_K_INDEPENDENT_HASH_FUNCTION_H
#define HASHTABLES_K_INDEPENDENT_HASH_FUNCTION_H

#include "hash_function.h"
#include <ctime>
#include <random>
#include <array>
#include <string>
#include <map>

template <class Key, int k>
class k_independent_hash_function : public hash_function<Key> {
private:
    std::array<uint64_t, k> a{{}};
    size_t m;
    const uint64_t p = 2305843009213693951L;
    std::random_device random_device;
    std::mt19937_64 generator;
    std::uniform_int_distribution<size_t > distribution;
//    std::normal_distribution<double > distribution;

public:
    explicit k_independent_hash_function(size_t m = 0) : m(m) {
        generator = std::mt19937_64(random_device());
        update(m);
    }

    k_independent_hash_function(const k_independent_hash_function &src) {
        a = src.a;
        m = src.m;

        generator = src.generator;
        distribution = src.distribution;
    }

    k_independent_hash_function &operator=(const k_independent_hash_function &src) {
        a = src.a;
        m = src.m;

        generator = src.generator;
        distribution = src.distribution;

        return *this;
    }

    void update(size_t m) override {
        this->m = m;

        distribution = std::uniform_int_distribution<size_t>(1, std::numeric_limits<uint64_t>::max());
//        distribution = std::normal_distribution<double >(1000000000, 10);

        for (int i = 0; i < k; i++) {
            a[i] = distribution(generator) % p;
//            a[i] = static_cast<uint64_t >(distribution(generator)) % p;
        }
    }

    ~k_independent_hash_function() override = default;

    uint64_t hash(const Key &key) override {
        uint64_t x = 1;
        uint64_t sum = 0;

        for (int i = 0; i < k; i++) {
            sum += a[i] * x;
            x *= key;
        }

        uint64_t moduloP = sum % p;
        uint64_t moduloM = moduloP % m;

        return moduloM;
    }
};
static std::map <char, uint64_t> alphabet = {{'a',0}, {'b', 1}, {'c',2}, {'d',3}, {'e',4}, {'f',5}, {'g',6}, {'h',7}, {'i',8}, {'j',9},
							{'k',10}, {'l',11}, {'m', 12}, {'n', 13}, {'o',14}, {'p', 15}, {'q', 16}, {'r', 17},{'s',18},
							{'t',19}, {'u',20}, {'v',21}, {'w', 22}, {'x', 23}, {'y', 24}, {'z', 25}};
template <int k>
class k_independent_hash_function<std::string, k> : public hash_function<std::string> {
private:
    std::array<uint64_t, k> a{};
    size_t m;
    const uint64_t p = 2305843009213693951L;
    std::random_device random_device;
    std::mt19937 generator;
    std::uniform_int_distribution<size_t > distribution;

public:
    explicit k_independent_hash_function(size_t m = 0) : m(m) {
        generator = std::mt19937(random_device());
        update(m);
    }

    k_independent_hash_function(const k_independent_hash_function &src) {
        a = src.a;
        m = src.m;

        generator = src.generator;
        distribution = src.distribution;
    }

    k_independent_hash_function &operator=(const k_independent_hash_function &src) {
        a = src.a;
        m = src.m;

        generator = src.generator;
        distribution = src.distribution;

        return *this;
    }

    void update(size_t m) override {
        this->m = m;

        distribution = std::uniform_int_distribution<size_t>(1, std::numeric_limits<uint64_t>::max());

        for (int i = 0; i < k; i++) {
            a[i] = distribution(generator) % p;
        }
    }

    ~k_independent_hash_function() override = default;

    uint64_t hash(const std::string &key) {
		uint64_t number = 0;
		uint64_t pow = 1;
		for (int i = 0; i < key.length(); i++) {
			number += alphabet.at(key[i]) * pow;
			pow *= 26;
		}

		uint64_t x = 1;
		uint64_t sum = 0;

		for (int i = 0; i < k; i++) {
			sum += a[i] * x;
			x *= number;
		}

		uint64_t moduloP = sum % p;
		uint64_t moduloM = moduloP % m;

		return moduloM;
    }
};

//template <>
//uint64_t k_independent_hash_function<std::string, 3>::hash(const std::string &key) {
//    size_t h = 0;
//
//    for (auto c : key) {
//        h = ((h*a[0]) + c) % p;
//    }
//
//    return h % m;
//}

#endif //HASHTABLES_K_INDEPENDENT_HASH_FUNCTION_H
