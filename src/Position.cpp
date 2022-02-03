#include "Position.hpp"
#include "PositionedRectangle.hpp"
#include "Rectangle.hpp"

Position::Position(size_t X, size_t Y) : X(X), Y(Y) {

}

PositionedRectangle Position::operator+(PositionedRectangle const& PositionedRect) const {
    return PositionedRectangle{Position{x() + PositionedRect.x(), y() + PositionedRect.y()}, Rectangle{PositionedRect.width(), PositionedRect.height()}};
}

Vector Position::operator-(Position const& Position) const {
    return Vector{substractSizes(x(), Position.x()), substractSizes(y(), Position.y())};
}

int64_t Position::substractSizes(size_t Size1, size_t Size2) const {
    if (Size1 <= Size2) {
        return Size2 - Size1;
    } else {
        return -static_cast<int64_t>(Size1 - Size2);
    }
}

size_t Position::x() const {
    return X;
}

size_t Position::y() const {
    return Y;
}
