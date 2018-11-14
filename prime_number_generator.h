#ifndef HASHTABLES_PRIME_NUMBER_GENERATOR_H
#define HASHTABLES_PRIME_NUMBER_GENERATOR_H


#include <numeric>
#include <random>

template <class T = unsigned int>
class prime_number_generator {

private:
    std::default_random_engine generator;
    std::uniform_int_distribution<T> distribution;

    unsigned long pow_modulo(T a, T n, T p) {
        T res = 1;
        a = a % p;

        while (n > 0) {
            if (n & 1)
                res = (res*a) % p;

            n = n>>1;
            a = (a*a) % p;
        }
        return res;
    }

    bool fermat_test(T n) {
        std::uniform_int_distribution<T> d(2, n-2);
        T a = d(generator);
        return pow_modulo(a, n-1, n) != 1;
    }

    bool is_prime(T n) {
        if (n % 2 == 0) {
            return false;
        }
        /*else if (fermat_test(n)) {
            return false;
        }*/

        return true;
    }

public:
    explicit prime_number_generator(unsigned long bits = 30) {
        auto max = static_cast<T>((1UL << bits) - 1);
        auto min = static_cast<T>((1UL << (bits - 1)) + 1);

        distribution = std::uniform_int_distribution<T>(min, max);
    };

    ~prime_number_generator() = default;

    T rand() {

        T random_number = 0;

        while (!is_prime(random_number)) random_number = distribution(generator);

        return random_number;
    }
};


#endif //HASHTABLES_PRIME_NUMBER_GENERATOR_H
