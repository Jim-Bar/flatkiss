#include "PositionedEllipse.hpp"

// Absolute value of the difference between two values (handles unsigned integers).
#define ABS_DIFF(Expr1, Expr2) (Expr1 > Expr2 ? Expr1 - (Expr2) : Expr2 - (Expr1))
#define SQUARE(Expr) (Expr * Expr)

PositionedEllipse::PositionedEllipse(Position const Position, Ellipse const Ellipse) : Pos(Position), Ell(Ellipse) {

}

bool PositionedEllipse::contains(Position const& Position) const {
    // https://math.stackexchange.com/a/76463
    return SQUARE(radiusY() * ABS_DIFF(Position.x(), x()))
        + SQUARE(radiusX() * ABS_DIFF(Position.y(), y()))
        <= SQUARE(radiusX() * radiusY());
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
