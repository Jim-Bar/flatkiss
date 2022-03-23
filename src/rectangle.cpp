#include "rectangle.hpp"

Rectangle::Rectangle(int64_t width, int64_t height)
    : width_{width}, height_{height} {}

int64_t Rectangle::height() const { return height_; }

int64_t Rectangle::width() const { return width_; }
