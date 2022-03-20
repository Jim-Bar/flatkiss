#include "vector.hpp"

Vector::Vector(int64_t dx, int64_t dy) : dx_(dx), dy_(dy) {}

Vector::Vector(uint64_t dx, uint64_t dy)
    : dx_(static_cast<int64_t>(dx)), dy_(static_cast<int64_t>(dy)) {}

int64_t Vector::dx() const { return dx_; }

int64_t Vector::dy() const { return dy_; }
