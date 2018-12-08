#ifndef HASHTABLES_WIKI_HASH_FUNCTION_H
#define HASHTABLES_WIKI_HASH_FUNCTION_H

#include "hash_function.h"
#include <random>
#include <ctime>

template <class Key>
class wiki_hash_function : public hash_function<Key> {
private:
	size_t a, b, m, mBig;
	std::random_device random_device;
	std::mt19937 generator;

public:
	explicit wiki_hash_function(size_t m) : m(m) {

		generator = std::mt19937(random_device());
		update(m);
	}

	void update(size_t m) {
		this->m = m;
		mBig = static_cast<size_t>(std::log2(m));

		auto maxA = static_cast<size_t >(std::pow(2, mBig));
		std::uniform_int_distribution<unsigned long> distributionA(1, maxA);
		a = distributionA(generator);

		auto maxB = static_cast<size_t >(std::pow(2, sizeof(Key)*8 - mBig));
		std::uniform_int_distribution<unsigned long> distributionB(1, maxB);
		b = distributionB(generator);
	}

	wiki_hash_function(const wiki_hash_function& src) {
		a = src.a;
		b = src.b;
		m = src.m;
	}

	~wiki_hash_function() override = default;

	size_t hash(const Key key) override {
		return (a*key + b) >> (sizeof(Key)*8 - mBig);
	}
};

#endif //HASHTABLES_GENERIC_HASH_FUNCTION_H
