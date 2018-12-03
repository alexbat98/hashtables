#ifndef HASHTABLES_HASHTABLE_H
#define HASHTABLES_HASHTABLE_H

#include <memory>
#include <list>
#include <functional>
#include "prime_number_generator.h"
#include <algorithm>
#include "hash_function.h"
#include "generic_hash_function.h"

template<class HolderKey, class HolderType>
struct CoockooItemHolder {
	const HolderKey &key;
	HolderType &data;

	ItemHolder(const HolderKey &key, HolderType &data) : key(key), data(data) {};

	ItemHolder() = default;

	friend bool operator==(const ItemHolder &a, const ItemHolder &b) {
		return a.key == b.key;
	}
};

template<class Key, class T, class A = std::allocator<CoockooItemHolder<Key, T>>>
class hash_table {
private:
	std::vector<CoockooItemHolder<T> > t1;
	std::vector<CoockooItemHolder<T> > t2;

	int a;

	void genericCoeff() {
		std::minstd_rand0 engine(static_cast<unsigned int>(seed));
		std::uniform_int_distribution<unsigned long> distribution(1, 100);
		a = distribution(engine);
	}

	Key h1(Key key) {
		return key % a;

	}

	Key h2(Key key) {
		return (key / a) % a;
	}

public:
	explicit coockoo_hash_table() {

	};

	coockoo_hash_table(const hash_table &src) noexcept {
		
	};

	coockoo_hash_table &operator=(const hash_table &src) {
		return *this;
	};

	virtual ~coockoo_hash_table() {
		
	};

	void add(const ItemHolder<Key, T> &item) {
		uint64_t size = static_cast<uint64_t>(3 * log(n));
		std::vector<CoockooItemHolder<T> > cycle(size);
		Key randomNumber;
		if (t1[h1(item.key)].key == 0) {
			t1[h1(key)].key = item;
		}
		else if (t2[h2(item.key)].key == 0) {
			t2[h2(key)].key = item;
		}
		else {
			std::default_random_engine generator;
			std::uniform_real_distribution<Key> uniform_distance(0, m);
			randomNumber = distribution(generator);
			int i = 0;
			cycle[0].key = t2[randomNumber].key;
			cycle[0].data = t2[randomNumber].data;
			t2[randomNumber].data = item.data;
			for (i = 0; i < size; i++) {
				if (t1[h1(cycle[i].key)].key == 0) {
					for (int j = i; j > 0; j--) {
						t1[h1(cycle[j].key)] = cycle[j].data;
					}
					break;
				}
				else {
					cycle[i + 1].key = t1[h1(cycle[i].key)].key;
					cycle[i + 1].data = t1[h1(cycle[i].key)].data;
				}
			}
			if (i == size) {
				genericCoeff();
				rehash(a);
				//увеличить размер таблицы
			}

		}

	}

	void add(const Key &key, T &item) {
		ItemHolder<Key, T> holder(key, item);
		add(holder);
	};

	void remove(const Key &key) {
		if (t1[h1(key)].key == key) {
			t1[h1(key)] = CoockooItemHolder();
		}
		else if (t2[h2(key)].key == key) {
			t2[h2(key)] = CoockooItemHolder();
		}
	};

	T &get(const Key &key) {
		if (t1[h1(key)].key == key) {
			return t1[h1(key)].data;
		}
		else if (t2[h2(key)].key == key) {
			return t2[h2(key)].data;
		}
	};

	bool has_key(const Key &key) {
		if (t1[h1(key)].key == key || t2[h2(key)].key == key) {
			return true;
		}
		return false;
	}

	// todo add parameter to rehash
	void rehash(int a) {

	}
};

#endif //HASHTABLES_HASHTABLE_H
