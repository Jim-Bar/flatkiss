#include "vector.hpp"

Vector::Vector(int64_t dx, int64_t dy) : dx_(dx), dy_(dy) {}

int64_t Vector::dx() const { return dx_; }

int64_t Vector::dy() const { return dy_; }
