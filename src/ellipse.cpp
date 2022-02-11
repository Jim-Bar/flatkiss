#include "ellipse.hpp"

Ellipse::Ellipse(size_t radius_x, size_t radius_y)
    : radius_x_(radius_x), radius_y_(radius_y) {}

size_t Ellipse::radiusX() const { return radius_x_; }

size_t Ellipse::radiusY() const { return radius_y_; }
