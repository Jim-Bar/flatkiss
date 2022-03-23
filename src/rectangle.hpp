#ifndef RECTANGLE_HPP_INCLUDED
#define RECTANGLE_HPP_INCLUDED

#include <cstdint>

/**
 * @brief A rectangle is just what it is: two dimensions, a width and a height.
 *
 * There is no position associated with a rectangle.
 */
class Rectangle {
 public:
  Rectangle(int64_t width, int64_t height);
  int64_t height() const;
  int64_t width() const;

 private:
  int64_t const height_;
  int64_t const width_;
};

#endif
