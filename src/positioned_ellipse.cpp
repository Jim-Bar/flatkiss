#include "positioned_ellipse.hpp"

/* Absolute value of the difference between two values (handles unsigned
 * integers). */
size_t absoluteDifference(size_t Value1, size_t Value2) {
  return Value1 > Value2 ? Value1 - Value2 : Value2 - Value1;
}

size_t square(size_t Value) { return Value * Value; }

PositionedEllipse::PositionedEllipse(Position const& Position,
                                     Ellipse const& Ellipse)
    : Pos(Position), Ell(Ellipse) {}

bool PositionedEllipse::contains(Position const& Position) const {
  // https://math.stackexchange.com/a/76463
  return square(radiusY() * absoluteDifference(Position.x(), x())) +
             square(radiusX() * absoluteDifference(Position.y(), y())) <=
         square(radiusX() * radiusY());
}

Ellipse const& PositionedEllipse::ellipse() const { return Ell; }

Position const& PositionedEllipse::position() const { return Pos; }

size_t PositionedEllipse::radiusX() const { return Ell.radiusX(); }

size_t PositionedEllipse::radiusY() const { return Ell.radiusY(); }

size_t PositionedEllipse::x() const { return Pos.x(); }

size_t PositionedEllipse::y() const { return Pos.y(); }
