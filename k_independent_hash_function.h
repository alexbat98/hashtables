#ifndef HASHTABLES_K_INDEPENDENT_HASH_FUNCTION_H
#define HASHTABLES_K_INDEPENDENT_HASH_FUNCTION_H

#include "hash_function.h"
#include <ctime>
#include <random>
#include <array>
#include <string>

template <class Key, int k>
class k_independent_hash_function : public hash_function<Key> {
private:
    std::array<uint64_t, k> a{{}};
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

template <int k>
class k_independent_hash_function<std::string, k> : public hash_function<std::string> {
private:
    uint64_t a{};
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

        a = distribution(generator) % p;
    }

    ~k_independent_hash_function() override = default;

    uint64_t hash(const std::string &key) override {
        size_t h = 0;

        for (auto c : key) {
            h = ((h*a) + c) % p;
        }

        return h % m;
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
