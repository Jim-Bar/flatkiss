#ifndef ELLIPSE_HPP_INCLUDED
#define ELLIPSE_HPP_INCLUDED

#include <cstddef>

class Ellipse {
 public:
  Ellipse(size_t RadiusX, size_t RadiusY);
  size_t radiusX() const;
  size_t radiusY() const;

 private:
  size_t const RadiusX;
  size_t const RadiusY;
};

#endif
