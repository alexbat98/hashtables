#ifndef HASHTABLES_OPEN_HASH_TABLE_H
#define HASHTABLES_OPEN_HASH_TABLE_H

#include <memory>
#include <cstdlib>
#include <list>
#include <functional>
#include "prime_number_generator.h"
#include "wiki_hash_function.h"
#include <algorithm>

template<class HolderKey, class HolderType>
struct OpenItemHolder {
    HolderKey key{};
    HolderType data{};
    bool isEmpty;

    OpenItemHolder() : isEmpty(true) {};

    OpenItemHolder(HolderKey key, HolderType &data)
        : key(key), data(data), isEmpty(false) {};

    OpenItemHolder(const OpenItemHolder &src) {
        key = src.key;
        data = src.data;
        isEmpty = src.isEmpty;
    }

    OpenItemHolder& operator=(const OpenItemHolder& src) {
        key = src.key;
        data = src.data;
        isEmpty = src.isEmpty;

        return *this;
    }

    friend bool operator==(const OpenItemHolder &a, const OpenItemHolder &b) {
        return a.key == b.key;
    }
};

template<class Key, class T, class A = std::allocator<OpenItemHolder<Key, T>>>
class open_hash_table {
private:
    std::vector<OpenItemHolder<Key, T>, A> mHashTable;

    size_t m;

    wiki_hash_function<Key> hashFunction;

public:
    explicit open_hash_table(size_t m = sizeof(Key)*10) : m(m), hashFunction(wiki_hash_function<Key>(m)) {

        mHashTable.reserve(m);

        for (size_t i = 0; i < m; i++) {
            mHashTable.emplace(mHashTable.begin() + i, OpenItemHolder<Key, T>());
        }
    };

    open_hash_table(const open_hash_table &src) noexcept {
        m = src.m;
        mHashTable = src.mHashTable;
    };

    open_hash_table &operator=(const open_hash_table &src) {
        return *this;
    };

    virtual ~open_hash_table() = default;

    void add(OpenItemHolder<Key, T> &item) {
        Key hashKey = hashFunction.hash(item.key);
        if (mHashTable[hashKey].isEmpty) {
            mHashTable[hashKey] = item;
        } else {
            size_t i = 1;
            while (!mHashTable[(hashKey + i) % m].isEmpty) {
                ++i;
            }
            mHashTable[(hashKey + i) % m] = item;
        }
    }

    void add(Key key, T item) {
        OpenItemHolder<Key, T> holder(key, item);
        add(holder);
    };

    void remove(Key key) {
        size_t i = 0;
        Key hashKey = hashFunction.hash(key);
        while (!mHashTable[(hashKey + i) % m].isEmpty) {
            if (mHashTable[(hashKey + i) % m].key == key) {
                mHashTable[(hashKey + i) % m] = OpenItemHolder<Key, T>();
                break;
            } else {
                ++i;
            }
        }
    };

    T &get(Key key) {
        size_t i = 0;
        Key hashKey = hashFunction.hash(key);
        while (!mHashTable[(hashKey + i) % m].isEmpty) {
            if (mHashTable[(hashKey + i) % m].key == key) {
                return mHashTable[(hashKey + i) % m].data;
            } else {
                ++i;
            }
        }
    };

    bool has_key(Key key) {
        size_t i = 0;
        Key hashKey = hashFunction.hash(key);
        while (!mHashTable[(hashKey + i) % m].isEmpty) {
            if (mHashTable[(hashKey + i) % m].key == key) {
                return true;
            } else {
                ++i;
            }
        }
        return false;
    }

    void rehash() {
        hashFunction = wiki_hash_function<Key>(m);

        auto &oldTable = mHashTable;

        mHashTable = std::vector<OpenItemHolder<Key, T>, A>();
        mHashTable.reserve(m);

        for (size_t i = 0; i < m; i++) {
            mHashTable.emplace(mHashTable.begin() + i, OpenItemHolder<Key, T>());
        }

        for (auto &item : oldTable) {
            add(item);
        }
    }
};
#endif //HASHTABLES_OPEN_HASH_TABLE_H
