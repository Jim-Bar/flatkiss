#include "vector.hpp"

Vector const Vector::ZERO{0, 0};

Vector::Vector(int64_t dx, int64_t dy) : dx_{dx}, dy_{dy} {}

bool Vector::operator!=(Vector const& other) const {
  return !(*this == other);
}

bool Vector::operator==(Vector const& other) const {
  return dx() == other.dx() && dy() == other.dy();
}

int64_t Vector::dx() const { return dx_; }

int64_t Vector::dy() const { return dy_; }
