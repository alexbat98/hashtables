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
struct ItemHolder {
  const HolderKey &key;
  HolderType &data;

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
  const int m;

  std::shared_ptr<hash_function<Key> > mHashFunction;

  uint64_t collision_counter;

  size_t max_list_length;

public:
  explicit hash_table(const int m = 7,
      std::shared_ptr<hash_function<Key> > hashFunction = std::make_shared<generic_hash_function<Key> >())
      : m(m), mHashFunction(hashFunction), collision_counter(0),
        max_list_length(0) {

    prime_number_generator<unsigned long> g;




    data = new std::list<ItemHolder<Key, T>, A>[m];
  };

  hash_table(const hash_table &src) noexcept {
    m = src.m;
    collision_counter = src.collision_counter;
    mHashFunction = src.mHashFunction;

    data = new std::list<ItemHolder<Key, T> >[m];

    for (int i = 0; i < m; i++) {
      data[i] = src.data[i];
    }
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

  void add(const ItemHolder<Key, T> &item) {
    size_t idx = mHashFunction->hash(item.key, (uint64_t) m);
    data[idx].push_back(item);

    max_list_length = std::max(data[idx].size(), max_list_length);

    if (data[idx].size() > 1) {
      collision_counter++;
    }
  }

  void add(const ItemHolder<Key, T> &&item, bool disable_rebase = false) {
    size_t idx = mHashFunction->hash(item.key, (uint64_t) m);
    data[idx].push_back(item);

    max_list_length = std::max(data[idx].size(), max_list_length);

    if (data[idx].size() > 1) {
      collision_counter++;
    }

    if (!disable_rebase && should_rebase(collision_counter, max_list_length)) {
      rehash();
    }
  }

  void add(const Key &key, T &item) {
    ItemHolder<Key, T> holder(key, item);
    add(holder);
  };

  void remove(const Key &key) {
    size_t idx = mHashFunction->hash(key, (uint64_t) m);

    for (auto item : data[idx]) {
      if (item.key == key) {
        data[idx].remove(item);
        break;
      }
    }
  };

  T &get(const Key &key) {
    size_t idx = mHashFunction->hash(key, (uint64_t) m);

    for (auto item : data[idx]) {
      if (item.key == key) {
        return item.data;
      }
    }

    T* res = new T;

    return *res;
  };

  bool has_key(const Key &key) {
    size_t idx = mHashFunction->hash(key, (uint64_t) m);

    bool has_key_flag = false;

    for (auto item : data[idx]) {
      if (item.key == key) {
        has_key_flag = true;
        break;
      }
    }

    return has_key_flag;
  }

  // todo add parameter to rehash
  void rehash() {

    mHashFunction = std::make_shared<generic_hash_function<Key> >();

    auto *old_table = data;
    data = new std::list<ItemHolder<Key, T>, A>[m];

    for (int i = 0; i < m; ++i) {
      for (auto item : old_table[i]) {
        add(std::move(item), true);
      }
    }

    delete[] old_table;
  }

  uint64_t collisions() {
    return collision_counter;
  }

  size_t max_chain_length() {
    return max_list_length;
  }
};

#endif //HASHTABLES_HASHTABLE_H
