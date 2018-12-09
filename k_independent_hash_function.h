#ifndef HASHTABLES_K_INDEPENDENT_HASH_FUNCTION_H
#define HASHTABLES_K_INDEPENDENT_HASH_FUNCTION_H

#include "hash_function.h"
#include <ctime>
#include <random>
#include <array>

template <class Key, int k>
class k_independent_hash_function : public hash_function<Key> {
private:
    std::array<uint64_t, k> a{{}};
    size_t m;
//    const uint64_t p = 2305843009213693951L;
    int currentP;
    const std::array<uint64_t, 5> primes{{691096906169011L, 979853562951413L, 146263628637547L, 355693655479801L, 923291713111753L}};
    std::random_device random_device;
    std::mt19937 generator;
    std::uniform_int_distribution<size_t > distribution;

public:
    explicit k_independent_hash_function(size_t m = 0) : m(m) {
        currentP = -1;
        generator = std::mt19937(random_device());
        update(m);
    }

    k_independent_hash_function(const k_independent_hash_function &src) {
        a = src.a;
        m = src.m;
        currentP = src.currentP;

//        random_device = src.random_device;
        generator = src.generator;
        distribution = src.distribution;
    }

    k_independent_hash_function &operator=(const k_independent_hash_function &src) {
        a = src.a;
        m = src.m;
        currentP = src.currentP;

//        random_device = src.random_device;
        generator = src.generator;
        distribution = src.distribution;

        return *this;
    }

    void update(size_t m) override {
        this->m = m;
        currentP = (currentP + 1) % 5;

        distribution = std::uniform_int_distribution<size_t>(1, primes[currentP] - 1);

        for (int i = 0; i < k; i++) {
            a[i] = distribution(generator);
        }
    }

    ~k_independent_hash_function() override = default;

    size_t hash(const Key key) override {
        size_t x = 1;
        size_t sum = 0;

        for (int i = 0; i < k; i++) {
            sum += a[i] * x;
            x *= key;
        }

        return (sum % primes[currentP]) % m;
    }
};

template <>
size_t k_independent_hash_function<std::string, 5>::hash(const std::string key) {
    size_t h = 0;

    for (auto c : key) {
        h = ((h*a[0]) + c) % primes[currentP];
    }

    return h % m;
}

#endif //HASHTABLES_K_INDEPENDENT_HASH_FUNCTION_H
