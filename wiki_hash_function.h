#ifndef HASHTABLES_WIKI_HASH_FUNCTION_H
#define HASHTABLES_WIKI_HASH_FUNCTION_H

#include "hash_function.h"
#include <random>
#include <ctime>

template <class Key>
class wiki_hash_function : public hash_function<Key> {
private:
	unsigned long a, b, m;

public:
	explicit wiki_hash_function(size_t m) : m(m) {
		long long seed = std::time(nullptr);
		std::minstd_rand0 engine(static_cast<unsigned int>(seed));

		auto maxA = static_cast<size_t >(std::pow(2, m - 1) - 3);
		std::uniform_int_distribution<unsigned long> distributionA(1, 2*maxA + 1);
		a = distributionA(engine);

		auto maxB = static_cast<size_t >(std::pow(2, sizeof(Key) - m));
		std::uniform_int_distribution<unsigned long> distributionB(1, maxB);
		b = distributionA(engine);
	}

	wiki_hash_function(const wiki_hash_function& src) {
		a = src.a;
		b = src.b;
		m = src.m;
	}

	~wiki_hash_function() override = default;

	size_t hash(const Key key) override {
		return (a*key + b) >> (sizeof(Key)*8 - m);
	}
};

#endif //HASHTABLES_GENERIC_HASH_FUNCTION_H
