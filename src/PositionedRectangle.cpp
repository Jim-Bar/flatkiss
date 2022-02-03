#include "PositionedRectangle.hpp"

PositionedRectangle::PositionedRectangle(Position const Position, Rectangle const Rectangle) : Pos(Position), Rect(Rectangle) {

}

size_t PositionedRectangle::height() const {
    return Rect.height();
}

bool PositionedRectangle::intersectsWith(PositionedRectangle const& Other) const {
    if (x() + width() <= Other.x()) {
        return false;
    }

    if (x() > Other.x() + Other.width()) {
        return false;
    }

    if (y() + height() <= Other.y()) {
        return false;
    }

    if (y() > Other.y() + Other.height()) {
        return false;
    }

    return true;
}

PositionedRectangle PositionedRectangle::operator+(Position const& Position) const {
    // Using the operator+ from the class Position.
    return Position + *this;
}

Position const& PositionedRectangle::position() const {
    return Pos;
}

size_t PositionedRectangle::width() const {
    return Rect.width();
}

size_t PositionedRectangle::x() const {
    return Pos.x();
}

size_t PositionedRectangle::y() const {
    return Pos.y();
}
