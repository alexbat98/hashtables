#ifndef HASHTABLES_WIKI_HASH_FUNCTION_H
#define HASHTABLES_WIKI_HASH_FUNCTION_H

#include "hash_function.h"
#include <random>
#include <ctime>

template <class Key>
class wiki_hash_function : public hash_function<Key> {
private:
	unsigned long a;

public:
	wiki_hash_function() {
		long long seed = std::time(nullptr);
		std::minstd_rand0 engine(static_cast<unsigned int>(seed));
		std::uniform_int_distribution<unsigned long> distribution(1, p - 1);

		a = distribution(engine);
	}

	wiki_hash_function(const generic_hash_function& src) {
		a = src.a;
	}

	~wiki_hash_function() override = default;

	size_t hash(const Key key, unsigned long m) override {
		return (a*key) >> (sizeof(Key)*8 - m);
	}
};

#endif //HASHTABLES_GENERIC_HASH_FUNCTION_H
