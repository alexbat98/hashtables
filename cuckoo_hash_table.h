#ifndef CHASHTABLES_HASHTABLE_H
#define CHASHTABLES_HASHTABLE_H

#include <memory>
#include <cstdlib>
#include <list>
#include <functional>
#include "prime_number_generator.h"
#include <algorithm>

template<class HolderKey, class HolderType>
struct CuckooItemHolder {
    HolderKey key{};
    HolderType data{};
    bool isEmpty;

    CuckooItemHolder() : isEmpty(true) {};

    CuckooItemHolder(HolderKey key, HolderType &data)
        : key(key), data(data), isEmpty(false) {};

    CuckooItemHolder(const CuckooItemHolder &src) {
        key = src.key;
        data = src.data;
        isEmpty = src.isEmpty;
    }

    CuckooItemHolder& operator=(const CuckooItemHolder& src) {
        key = src.key;
        data = src.data;
        isEmpty = src.isEmpty;

        return *this;
    }

    friend bool operator==(const CuckooItemHolder &a, const CuckooItemHolder &b) {
        return a.key == b.key;
    }
};

template<class Key, class T, class A = std::allocator<CuckooItemHolder<Key, T>>>
class cuckoo_hash_table {
private:
    std::vector<CuckooItemHolder<Key, T>, A> t1;
    std::vector<CuckooItemHolder<Key, T>, A> t2;

    size_t a, m;

    void generateCoefficient() {
        std::minstd_rand0 engine(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_int_distribution<unsigned long> distribution(1, m);
        a = distribution(engine);
    }

    Key h1(Key key) {
        return key % a;

    }

    Key h2(Key key) {
        return (key / a) % a;
    }

public:
    explicit cuckoo_hash_table(size_t m = sizeof(Key)*10) : m(m), a(0) {
        generateCoefficient();

        t1.reserve(m);
        t2.reserve(m);

        for (size_t i = 0; i < m; i++) {
            t1.emplace(t1.begin() + i, CuckooItemHolder<Key, T>());
            t2.emplace(t2.begin() + i, CuckooItemHolder<Key, T>());
        }
    };

    cuckoo_hash_table(const cuckoo_hash_table &src) noexcept {

    };

    cuckoo_hash_table &operator=(const cuckoo_hash_table &src) {
        return *this;
    };

    virtual ~cuckoo_hash_table() = default;

    void add(CuckooItemHolder<Key, T> &item) {

        if (t1[h1(item.key)].isEmpty == true) {
            t1[h1(item.key)] = item;
        } else if (t2[h2(item.key)].isEmpty == true) {
            t2[h2(item.key)] = item;
        } else {
            auto maxIterations = static_cast<uint64_t>(3 * log(std::numeric_limits<Key>::max()));
            std::vector<CuckooItemHolder<Key, T> > cycle;
            cycle.reserve(maxIterations);
//            for (size_t i = 0; i < maxIterations; i++) {
//                cycle.emplace()
//            }
            size_t randomNumber;

            std::default_random_engine generator;
            std::uniform_int_distribution<Key> distribution(0, m);
            randomNumber = distribution(generator);
            size_t i = 0;
            cycle[0] = t2[randomNumber];
//            cycle[0].data = t2[randomNumber].data;
            t2[randomNumber] = item;
            for (i = 0; i < maxIterations; i++) {
                if (t1[h1(cycle[i].key)].isEmpty == true) {
                    for (size_t j = i; j > 0; j--) {
                        t1[h1(cycle[j].key)] = cycle[j];
                    }
                    break;
                } else {
                    cycle[i + 1] = t1[h1(cycle[i].key)];
//                    cycle[i + 1].data = mHashTable[h1(cycle[i].key)].data;
                }
            }
            if (i == maxIterations) {
//                generateCoefficient();
                rehash();
                add(item);
            }

        }

    }

    void add(Key key, T item) {
        CuckooItemHolder<Key, T> holder(key, item);
        add(holder);
    };

    void remove(Key key) {
        if (t1[h1(key)].key == key) {
            t1[h1(key)] = CuckooItemHolder<Key, T>();
        } else if (t2[h2(key)].key == key) {
            t2[h2(key)] = CuckooItemHolder<Key, T>();
        }
    };

    T &get(Key key) {
        if (t1[h1(key)].key == key && !t1[h1(key)].isEmpty) {
            return t1[h1(key)].data;
        } else if (t2[h2(key)].key == key && !t2[h2(key)].isEmpty) {
            return t2[h2(key)].data;
        }
    };

    bool has_key(Key key) {
        return (t1[h1(key)].key == key && !t1[h1(key)].isEmpty) || (t2[h2(key)].key == key && !t2[h2(key)].isEmpty);
    }

    void rehash() {
        generateCoefficient();
        auto &t1Old = t1;
        auto &t2Old = t2;

        t1 = std::vector<CuckooItemHolder<Key, T>, A>();
        t2 = std::vector<CuckooItemHolder<Key, T>, A>();
        t1.reserve(m);
        t2.reserve(m);

        for (size_t i = 0; i < m; i++) {
            t1.emplace(t1.begin() + i, CuckooItemHolder<Key, T>());
            t2.emplace(t2.begin() + i, CuckooItemHolder<Key, T>());
        }

        for (auto &item : t1Old) {
            add(item);
        }

        for (auto &item : t2Old) {
            add(item);
        }
    }
};

#endif //CHASHTABLES_HASHTABLE_H
