#ifndef HASHTABLES_GENERIC_HASH_FUNCTION_H
#define HASHTABLES_GENERIC_HASH_FUNCTION_H

#include "hash_function.h"
#include "prime_number_generator.h"
#include <ctime>

template <class Key>
class generic_hash_function : public hash_function<Key> {
private:
  unsigned long a, b, p;

public:
  generic_hash_function() {
    prime_number_generator<unsigned long> g;
    p = g.rand();
    long long seed = std::time(nullptr);
    std::minstd_rand0 engine(static_cast<unsigned int>(seed));
    std::uniform_int_distribution<unsigned long> distribution(1, p - 1);

    a = distribution(engine);
    b = distribution(engine);
  }

  generic_hash_function(const generic_hash_function& src) {
    a = src.a;
    b = src.b;
    p = src.p;
  }

  ~generic_hash_function() override = default;

  size_t hash(const Key key, unsigned long m) override {
    return ((a * key + b) % p) % m;
  }
};

#endif //HASHTABLES_GENERIC_HASH_FUNCTION_H
