#include "Rectangle.hpp"

Rectangle::Rectangle(size_t Width, size_t Height)
    : Width(Width), Height(Height) {}

size_t Rectangle::height() const { return Height; }

size_t Rectangle::width() const { return Width; }
