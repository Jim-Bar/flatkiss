#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED

#include <cstdint>

/**
 * @brief A delta in pixels.
 *
 * Note that this is always in pixels. This class must not be used for things
 * measured in tiles.
 */
class Vector {
 public:
  Vector(int64_t dx, int64_t dy);
  bool operator!=(Vector const& other) const;
  bool operator==(Vector const& other) const;
  int64_t dx() const;
  int64_t dy() const;

  static Vector const kZero;

 private:
  int64_t const dx_;
  int64_t const dy_;
};

#endif
