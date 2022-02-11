#ifndef ELLIPSE_HPP_INCLUDED
#define ELLIPSE_HPP_INCLUDED

#include <cstddef>

class Ellipse {
 public:
  Ellipse(size_t radius_x, size_t radius_y);
  size_t radiusX() const;
  size_t radiusY() const;

 private:
  size_t const radius_x_;
  size_t const radius_y_;
};

#endif
