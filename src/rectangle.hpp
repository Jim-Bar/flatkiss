#ifndef RECTANGLE_HPP_INCLUDED
#define RECTANGLE_HPP_INCLUDED

#include <cstddef>

/**
 * @brief A rectangle is just what it is: two dimensions, a width and a height.
 *
 * There is no position associated with a rectangle.
 */
class Rectangle {
 public:
  Rectangle(size_t Width, size_t Height);
  size_t height() const;
  size_t width() const;

 private:
  size_t const Height;
  size_t const Width;
};

#endif
