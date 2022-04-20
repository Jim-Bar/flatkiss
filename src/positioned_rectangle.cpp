#include "positioned_rectangle.hpp"

#include <utility>

#include "positioned_ellipse.hpp"

using std::move;

PositionedRectangle::PositionedRectangle(Position const& position,
                                         Rectangle const& rectangle)
    : position_{position}, rectangle_{rectangle} {}

PositionedRectangle::PositionedRectangle(PositionedRectangle const& other)
    : position_{other.position()}, rectangle_{other.rectangle()} {}

PositionedRectangle::PositionedRectangle::PositionedRectangle(
    PositionedRectangle&& other)
    : position_{move(other.position_)}, rectangle_{move(other.rectangle_)} {}

PositionedRectangle::~PositionedRectangle() {}

int64_t PositionedRectangle::height() const { return rectangle_.height(); }

bool PositionedRectangle::intersectsWith(PositionedEllipse const& other) const {
  /* Checking that a rectangle (or even line for that matter) intersects with an
   * ellipse is no easy task. However this is a simpler case because both the
   * rectangle and ellipse are axis aligned (i.e. not rotated). So the check
   * boils down to:
   * - test whether the ellipse contains one of the four points of the rectangle
   * - test for each vertex and co-vertex of the ellipse whether it is on the
   * other side of the closest side of the rectangle, and within the bounds of
   * the two adjacent sides of the rectangle
   *
   * Note for the second test that checking whether each (co-)vertex is
   * contained in the rectangle is not enough: if the rectangle moved too fast
   * (or is too thin), the (co-)vertex could end up beyond the rectangle. The
   * second test is needed when the rectangle is larger than the ellipse.
   *
   * Okay, reading this could sounds complex, but it is actually not that
   * complex. Just take a pen a draw ellipses and rectangles. */

  // First test.
  if (other.contains(position()) ||
      other.contains(position() + Vector{width(), 0}) ||
      other.contains(position() + Vector(0, height())) ||
      other.contains(position() + Vector{width(), height()})) {
    return true;
  }

  // Second test, part one.
  if (other.x() >= x() && other.x() <= x() + width()) {
    if (other.y() + other.radiusY() >= y() ||
        other.y() >= y() + other.radiusY()) {
      return true;
    }
  }

  // End of the second test.
  if (other.y() >= y() && other.y() <= y() + height()) {
    if (other.x() + other.radiusX() >= x() ||
        other.x() >= x() + other.radiusX()) {
      return true;
    }
  }

  return false;
}

bool PositionedRectangle::intersectsWith(
    PositionedRectangle const& other) const {
  if (x() + width() <= other.x()) {
    return false;
  }

  if (x() > other.x() + other.width()) {
    return false;
  }

  if (y() + height() <= other.y()) {
    return false;
  }

  if (y() > other.y() + other.height()) {
    return false;
  }

  return true;
}

PositionedRectangle PositionedRectangle::operator+(Vector const& vector) const {
  return PositionedRectangle{position() + vector, rectangle()};
}

Position const& PositionedRectangle::position() const { return position_; }

void PositionedRectangle::position(Position&& new_position) {
  position_ = move(new_position);
}

Rectangle const& PositionedRectangle::rectangle() const { return rectangle_; }

int64_t PositionedRectangle::width() const { return rectangle_.width(); }

int64_t PositionedRectangle::x() const { return position_.x(); }

void PositionedRectangle::x(int64_t newX) { position_.x(newX); }

int64_t PositionedRectangle::y() const { return position_.y(); }

void PositionedRectangle::y(int64_t newY) { position_.y(newY); }
