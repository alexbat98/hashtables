#ifndef HASHTABLES_HASH_FUNCTION_H
#define HASHTABLES_HASH_FUNCTION_H

#include <cstddef>
#include <cstdint>

template <class Key>
class hash_function {
public:
  virtual ~hash_function() = default;
  virtual void update(size_t m) = 0;
  virtual uint64_t hash(const Key &key) = 0;
};

#endif //HASHTABLES_HASH_FUNCTION_H
