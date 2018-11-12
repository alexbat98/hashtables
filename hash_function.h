#ifndef HASHTABLES_HASH_FUNCTION_H
#define HASHTABLES_HASH_FUNCTION_H

#include <cstddef>

template <class Key>
class hash_function {
public:
  virtual ~hash_function() = default;
  virtual size_t hash(const Key key, unsigned long m) = 0;
};

#endif //HASHTABLES_HASH_FUNCTION_H
