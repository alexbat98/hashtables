#ifndef CHASHTABLES_HASHTABLE_H
#define CHASHTABLES_HASHTABLE_H

#include <memory>
#include <cstdlib>
#include <list>
#include <functional>
#include "k_independent_hash_function.h"
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

    size_t m, maxIterations;

    k_independent_hash_function<Key, 5> hash_function1;
    k_independent_hash_function<Key, 5> hash_function2;

    size_t capacity{};

public:
    explicit cuckoo_hash_table(size_t m = 10000) : m(m*2) {
        hash_function1.update(m);
        hash_function2.update(m);

        maxIterations = static_cast<size_t>(3*std::log(m));

        t1.reserve(m);
        t2.reserve(m);

        for (size_t i = 0; i < m; i++) {
            t1.emplace(t1.begin() + i, CuckooItemHolder<Key, T>());
            t2.emplace(t2.begin() + i, CuckooItemHolder<Key, T>());
        }
    };

    cuckoo_hash_table(const cuckoo_hash_table &src) noexcept {
        t1 = src.t1;
        t2 = src.t2;

        m = src.m;
        maxIterations = src.maxIterations;

        hash_function1 = src.hash_function1;
        hash_function2 = src.hash_function2;

        capacity = src.capacity;
    };

    cuckoo_hash_table &operator=(const cuckoo_hash_table &src) {
        t1 = src.t1;
        t2 = src.t2;

        m = src.m;
        maxIterations = src.maxIterations;

        hash_function1 = src.hash_function1;
        hash_function2 = src.hash_function2;

        capacity = src.capacity;

        return *this;
    };

    virtual ~cuckoo_hash_table() = default;

    void add(CuckooItemHolder<Key, T> item) {
        size_t h1 = hash_function1.hash(item.key);
        size_t h2 = hash_function2.hash(item.key);

        if (has_key(item.key)) {
            get(item.key) = item.data;
            return;
        }

        if (t1[hash_function1.hash(item.key)].isEmpty) {
            t1[hash_function1.hash(item.key)] = item;
        } else if (t2[hash_function2.hash(item.key)].isEmpty) {
            t2[hash_function2.hash(item.key)] = item;
        } else {
            for (size_t i = 0; i < maxIterations; i++) {
                CuckooItemHolder<Key, T> tempItem = t1[hash_function1.hash(item.key)];
                t1[hash_function1.hash(item.key)] = item;
                item = tempItem;

                if (item.isEmpty) {
                    capacity++;
                    return;
                }

                tempItem = t2[hash_function2.hash(item.key)];
                t2[hash_function2.hash(item.key)] = item;
                item = tempItem;

                if (item.isEmpty) {
                    capacity++;
                    return;
                }

            }

            rehash();
            add(item);
        }
        capacity++;
    }

    void add(Key key, T item) {
        CuckooItemHolder<Key, T> holder(key, item);
        add(holder);
    };

    void remove(Key key) {
        if (t1[hash_function1.hash(key)].key == key) {
            t1[hash_function1.hash(key)] = CuckooItemHolder<Key, T>();
        } else if (t2[hash_function2.hash(key)].key == key) {
            t2[hash_function2.hash(key)] = CuckooItemHolder<Key, T>();
        }
    };

    T &get(Key key) {
        if (t1[hash_function1.hash(key)].key == key && !t1[hash_function1.hash(key)].isEmpty) {
            return t1[hash_function1.hash(key)].data;
        } else if (t2[hash_function2.hash(key)].key == key && !t2[hash_function2.hash(key)].isEmpty) {
            return t2[hash_function2.hash(key)].data;
        }
    };

    __attribute__ ((optnone)) bool has_key(Key key) {
        size_t h1 = hash_function1.hash(key);
        size_t h2 = hash_function2.hash(key);
        return (t1[h1].key == key && !t1[h1].isEmpty)
                || (t2[h2].key == key && !t2[h2].isEmpty);
    }

    void rehash() {

        if (capacity > m / 4) {
            m *= 2;
        }

        hash_function1.update(m);
        hash_function2.update(m);

        //maxIterations = static_cast<size_t>(3*std::log2(m));

        auto t1Old = t1;
        auto t2Old = t2;

        t1 = std::vector<CuckooItemHolder<Key, T>, A>();
        t2 = std::vector<CuckooItemHolder<Key, T>, A>();
        t1.reserve(m);
        t2.reserve(m);

        for (size_t i = 0; i < m; i++) {
            t1.emplace(t1.begin() + i, CuckooItemHolder<Key, T>());
            t2.emplace(t2.begin() + i, CuckooItemHolder<Key, T>());
        }

        for (auto item : t1Old) {
            if (!item.isEmpty) {
                add(item);
            }
        }

        for (auto item : t2Old) {
            if (!item.isEmpty) {
                add(item);
            }
        }
    }
};

#endif //CHASHTABLES_HASHTABLE_H
