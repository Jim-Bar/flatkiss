#include "ellipse.hpp"

Ellipse::Ellipse(size_t RadiusX, size_t RadiusY)
    : RadiusX(RadiusX), RadiusY(RadiusY) {}

size_t Ellipse::radiusX() const { return RadiusX; }

size_t Ellipse::radiusY() const { return RadiusY; }
