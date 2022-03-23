#include "ellipse.hpp"

Ellipse::Ellipse(int64_t radius_x, int64_t radius_y)
    : radius_x_{radius_x}, radius_y_{radius_y} {}

int64_t Ellipse::radiusX() const { return radius_x_; }

int64_t Ellipse::radiusY() const { return radius_y_; }
