#ifndef HASHTABLES_HASH_FUNCTION_H
#define HASHTABLES_HASH_FUNCTION_H

#include <cstddef>

template <class Key>
class hash_function {
public:
  virtual ~hash_function() = default;
  virtual size_t hash(Key key) = 0;
  virtual void update(size_t m) {};
};

#endif //HASHTABLES_HASH_FUNCTION_H
