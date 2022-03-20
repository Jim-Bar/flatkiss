#include "positioned_ellipse.hpp"

#include <utility>

using std::move;

/* Absolute value of the difference between two values (handles unsigned
 * integers). */
size_t absoluteDifference(size_t value_1, size_t value_2) {
  return value_1 > value_2 ? value_1 - value_2 : value_2 - value_1;
}

size_t square(size_t value) { return value * value; }

PositionedEllipse::PositionedEllipse(Position const& position,
                                     Ellipse const& ellipse)
    : position_(position), ellipse_(ellipse) {}

PositionedEllipse::PositionedEllipse(PositionedEllipse const& other)
    : position_(other.position_), ellipse_(other.ellipse_) {}

PositionedEllipse::PositionedEllipse(PositionedEllipse&& other)
    : position_(move(other.position_)), ellipse_(move(other.ellipse_)) {}

PositionedEllipse::~PositionedEllipse() {}

bool PositionedEllipse::contains(Position const& Position) const {
  // https://math.stackexchange.com/a/76463
  return square(radiusY() * absoluteDifference(Position.x(), x())) +
             square(radiusX() * absoluteDifference(Position.y(), y())) <=
         square(radiusX() * radiusY());
}

Ellipse const& PositionedEllipse::ellipse() const { return ellipse_; }

Position const& PositionedEllipse::position() const { return position_; }

size_t PositionedEllipse::radiusX() const { return ellipse_.radiusX(); }

size_t PositionedEllipse::radiusY() const { return ellipse_.radiusY(); }

size_t PositionedEllipse::x() const { return position_.x(); }

size_t PositionedEllipse::y() const { return position_.y(); }
