#include "PositionedEllipse.hpp"
#include "PositionedRectangle.hpp"

PositionedRectangle::PositionedRectangle(Position const Position, Rectangle const Rectangle) : Pos(Position), Rect(Rectangle) {

}

size_t PositionedRectangle::height() const {
    return Rect.height();
}

bool PositionedRectangle::intersectsWith(PositionedEllipse const& Other) const {
    // https://math.stackexchange.com/a/76463
    // FIXME: Will not work when x() < Other.x() (same with y).
    return Other.radiusY() * Other.radiusY() * (x() - Other.x()) * (x() - Other.x())
        + Other.radiusX() * Other.radiusX() * (y() - Other.y()) * (y() - Other.y())
        <= Other.radiusX() * Other.radiusX() * Other.radiusY() * Other.radiusY();
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

PositionedRectangle PositionedRectangle::operator+(Vector const& Vector) const {
    return PositionedRectangle{position() + Vector, rectangle()};
}

Position const& PositionedRectangle::position() const {
    return Pos;
}

Rectangle const& PositionedRectangle::rectangle() const {
    return Rect;
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
