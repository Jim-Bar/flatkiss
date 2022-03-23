#ifndef ELLIPSE_HPP_INCLUDED
#define ELLIPSE_HPP_INCLUDED

#include <cstdint>

class Ellipse {
 public:
  Ellipse(int64_t radius_x, int64_t radius_y);
  int64_t radiusX() const;
  int64_t radiusY() const;

 private:
  int64_t const radius_x_;
  int64_t const radius_y_;
};

#endif
