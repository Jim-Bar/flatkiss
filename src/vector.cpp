#include "vector.hpp"

Vector::Vector(int64_t Dx, int64_t Dy) : Dx(Dx), Dy(Dy) {}

int64_t Vector::dx() const { return Dx; }

int64_t Vector::dy() const { return Dy; }
