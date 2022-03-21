#include "rectangle.hpp"

Rectangle::Rectangle(size_t width, size_t height)
    : width_{width}, height_{height} {}

size_t Rectangle::height() const { return height_; }

size_t Rectangle::width() const { return width_; }
