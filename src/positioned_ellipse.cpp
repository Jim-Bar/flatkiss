#include "positioned_ellipse.hpp"

#include <cmath>
#include <utility>

using std::abs;
using std::move;

int64_t square(int64_t value) { return value * value; }

PositionedEllipse::PositionedEllipse(Position const& position,
                                     Ellipse const& ellipse)
    : position_{position}, ellipse_{ellipse} {}

PositionedEllipse::PositionedEllipse(PositionedEllipse const& other)
    : position_{other.position_}, ellipse_{other.ellipse_} {}

PositionedEllipse::PositionedEllipse(PositionedEllipse&& other)
    : position_{move(other.position_)}, ellipse_{move(other.ellipse_)} {}

PositionedEllipse::~PositionedEllipse() {}

bool PositionedEllipse::contains(Position const& Position) const {
  // https://math.stackexchange.com/a/76463
  return square(radiusY() * abs(Position.x() - x())) +
             square(radiusX() * abs(Position.y() - y())) <=
         square(radiusX() * radiusY());
}

Ellipse const& PositionedEllipse::ellipse() const { return ellipse_; }

Position const& PositionedEllipse::position() const { return position_; }

int64_t PositionedEllipse::radiusX() const { return ellipse_.radiusX(); }

int64_t PositionedEllipse::radiusY() const { return ellipse_.radiusY(); }

int64_t PositionedEllipse::x() const { return position_.x(); }

int64_t PositionedEllipse::y() const { return position_.y(); }
