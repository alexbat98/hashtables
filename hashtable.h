#ifndef HASHTABLES_HASHTABLE_H
#define HASHTABLES_HASHTABLE_H

#include <memory>
#include <list>
#include <functional>

template<class HolderKey, class HolderType>
struct ItemHolder {
    const HolderKey &key;
    HolderType &data;

    ItemHolder(const HolderKey& key, HolderType& data) : key(key), data(data) {};

    ItemHolder() = default;

    friend bool operator==(const ItemHolder& a, const ItemHolder& b) {
        return a.key == b.key;
    }
};

template<class Key, class T, class A = std::allocator<ItemHolder<Key, T>> >
class hashtable {
private:

    std::list<ItemHolder<Key, T>, A> *data;
    const int m;

    static unsigned int default_hash_function(Key key, uint64_t a, uint64_t b, uint64_t p, uint64_t m) {
        return ((a*key + b) % p) % m;
    }

    const std::function<unsigned int(Key, uint64_t, uint64_t, uint64_t, uint64_t)> &hash_function;

    unsigned long a, b, p;

public:
    explicit hashtable(const int m = 7,
                       const std::function<unsigned int(Key, uint64_t, uint64_t,
                                                        uint64_t, uint64_t)> &hash_function = default_hash_function)
            : m(m), hash_function(hash_function) {
        p = 13; // todo replace with prime random number generator
        a = 2;
        b = 5;
        data = new std::list<ItemHolder<Key, T>, A>[m];
    };

    hashtable(const hashtable &src) noexcept {
        a = src.a;
        b = src.b;
        p = src.p;
        m = src.m;

        data = new std::list<ItemHolder<Key, T> >[m];

        for (int i = 0; i < m; i++) {
            data[i] = src.data[i];
        }
    };

    hashtable(const hashtable &&src) noexcept {
        a = src.a;
        b = src.b;
        p = src.p;
        m = src.m;

        data = src.data;
    }

    hashtable &operator=(const hashtable &src) = delete;

    virtual ~hashtable() {
        delete [] data;
    };

    void add(const Key &key, T &item) {
        unsigned int idx = hash_function(key, a, b, p, (uint64_t) m);
        ItemHolder<Key, T> holder(key, item);
        data[idx].push_back(std::move(holder));
    };

    void remove(const Key &key) {
        unsigned int idx = hash_function(key, a, b, p, (uint64_t) m);

        for (auto item : data[idx]) {
            if (item.key == key) {
                data[idx].remove(item);
                break;
            }
        }
    };

    T &get(const Key &key) {
        unsigned int idx = hash_function(key, a, b, p, (uint64_t) m);

        for (auto item : data[idx]) {
            if (item.key == key) {
                return item.data;
            }
        }

        throw std::runtime_error("");
    };

    bool has_key(const Key &key) {
        unsigned int idx = hash_function(key, a, b, p, (uint64_t) m);

        bool has_key_flag = false;

        for (auto item : data[idx]) {
            if (item.key == key) {
                has_key_flag = true;
                break;
            }
        }

        return has_key_flag;
    }
};


#endif //HASHTABLES_HASHTABLE_H
