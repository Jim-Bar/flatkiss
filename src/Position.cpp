#include "Position.hpp"
#include "PositionedRectangle.hpp"
#include "Rectangle.hpp"

Position::Position(size_t X, size_t Y) : X(X), Y(Y) {

}

PositionedRectangle Position::operator+(PositionedRectangle const& PositionedRect) const {
    return PositionedRectangle{Position{x() + PositionedRect.x(), y() + PositionedRect.y()}, Rectangle{PositionedRect.width(), PositionedRect.height()}};
}

size_t Position::x() const {
    return X;
}

size_t Position::y() const {
    return Y;
}
