#ifndef HASHTABLES_HASHTABLE_H
#define HASHTABLES_HASHTABLE_H

#include <memory>
#include <list>
#include <functional>
#include "prime_number_generator.h"
#include <algorithm>
#include "hash_function.h"
#include "wiki_hash_function.h"
#include "k_independent_hash_function.h"

template<class HolderKey, class HolderType>
struct ItemHolder {
    const HolderKey key;
    HolderType data;

    ItemHolder(const HolderKey &key, HolderType &data) : key(key), data(data) {};

    ItemHolder() = default;

    friend bool operator==(const ItemHolder &a, const ItemHolder &b) {
        return a.key == b.key;
    }
};

template<class Key, class T, class A = std::allocator<ItemHolder<Key, T>>>
class hash_table {
private:

    std::list<ItemHolder<Key, T>, A> *data;
    size_t m;

    std::shared_ptr<hash_function<Key> > mHashFunction;

    size_t max_list_length;
    size_t capacity;
    size_t last_rehash_capacity;
    size_t rehash_trigger_length;

public:
    explicit hash_table(
        size_t m = 0,
        std::shared_ptr<hash_function<Key> > hashFunction = std::make_shared<
            k_independent_hash_function<Key, 2> >(0))
        : m(m), mHashFunction(hashFunction), capacity(0), last_rehash_capacity(0),
          max_list_length(0) {

        if (m == 0) {
            m = 100000;
        }

        rehash_trigger_length = static_cast<size_t>(3 * std::sqrt(m));

        mHashFunction->update(m);

        prime_number_generator<unsigned long> g;

        data = new std::list<ItemHolder<Key, T>, A>[m];
    };

    hash_table(const hash_table &src) noexcept {
        m = src.m;
        max_list_length = src.max_list_length;
        capacity = src.capacity;
        last_rehash_capacity = src.last_rehash_capacity;
        rehash_trigger_length = src.rehash_trigger_length;
        mHashFunction = src.mHashFunction;

        data = new std::list<ItemHolder<Key, T> >[m];

        for (int i = 0; i < m; i++) {
            data[i] = src.data[i];
        }
    };

    hash_table(hash_table &&src) noexcept {
        m = src.m;
        max_list_length = src.max_list_length;
        capacity = src.capacity;
        last_rehash_capacity = src.last_rehash_capacity;
        rehash_trigger_length = src.rehash_trigger_length;
        mHashFunction = src.mHashFunction;
        src.mHashFunction = nullptr;

        data = src.data;

        src.data = nullptr;
    };

    hash_table &operator=(hash_table &&src) {

        m = src.m;
        max_list_length = src.max_list_length;
        capacity = src.capacity;
        last_rehash_capacity = src.last_rehash_capacity;
        rehash_trigger_length = src.rehash_trigger_length;
        mHashFunction = src.mHashFunction;
        src.mHashFunction = nullptr;

        data = src.data;

        src.data = nullptr;

        return *this;
    };

    hash_table &operator=(const hash_table &src) {

        if (m != src.m) {
            delete[] data;
            data = new std::list<ItemHolder<Key, T> >[src.m];
        }

        mHashFunction = src.mHashFunction;
        m = src.m;

        for (int i = 0; i < m; i++) {
            data[i] = src.data[i];
        }

        return *this;
    };

    virtual ~hash_table() {
        delete[] data;
    };

    void add(ItemHolder<Key, T> item, bool no_rehash = false) {
        size_t idx = mHashFunction->hash(item.key);
        data[idx].push_back(item);

        capacity++;
        max_list_length = std::max(data[idx].size(), max_list_length);

        if (!no_rehash && max_list_length > rehash_trigger_length
            && capacity - last_rehash_capacity > m / 15) {
            last_rehash_capacity = capacity;
            rehash();
        }
    }

    void add(const Key key, T item) {
        ItemHolder<Key, T> holder(key, item);
        add(holder);
    };

    void remove(const Key &key) {
        size_t idx = mHashFunction->hash(key);

        for (auto item : data[idx]) {
            if (item.key == key) {
                data[idx].remove(item);
                break;
            }
        }
    };

    T &get(const Key &key) {
        size_t idx = mHashFunction->hash(key);

        for (auto item : data[idx]) {
            if (item.key == key) {
                return item.data;
            }
        }

        T *res = new T;

        return *res;
    };

    bool has_key(const Key &key) {
        size_t idx = mHashFunction->hash(key);

        bool has_key_flag = false;

        for (auto item : data[idx]) {
            if (item.key == key) {
                has_key_flag = true;
                break;
            }
        }

        return has_key_flag;
    }

    void rehash() {

        capacity = 0;
        max_list_length = 0;

        size_t oldM = m;

        if (capacity > static_cast<size_t>(0.7 * m)) {
            m *= 2;
        }

        mHashFunction->update(m);

        auto *old_table = data;
        data = new std::list<ItemHolder<Key, T>, A>[m];

        for (int i = 0; i < oldM; ++i) {
            for (auto &item : old_table[i]) {
                add(item, true);
            }
        }

        delete[] old_table;
    }
};

#endif //HASHTABLES_HASHTABLE_H
