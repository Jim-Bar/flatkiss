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
  Vector(int64_t Dx, int64_t Dy);
  int64_t dx() const;
  int64_t dy() const;

 private:
  int64_t const Dx;
  int64_t const Dy;
};

#endif
