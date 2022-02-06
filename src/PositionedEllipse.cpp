#include "PositionedEllipse.hpp"

PositionedEllipse::PositionedEllipse(Position const Position, Ellipse const Ellipse) : Pos(Position), Ell(Ellipse) {

}

Ellipse const& PositionedEllipse::ellipse() const {
    return Ell;
}

Position const& PositionedEllipse::position() const {
    return Pos;
}

size_t PositionedEllipse::radiusX() const {
    return Ell.radiusX();
}

size_t PositionedEllipse::radiusY() const {
    return Ell.radiusY();
}

size_t PositionedEllipse::x() const {
    return Pos.x();
}

size_t PositionedEllipse::y() const {
    return Pos.y();
}
