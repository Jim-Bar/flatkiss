#include "positioned_rectangle.hpp"

#include <utility>

#include "positioned_ellipse.hpp"

using std::move;

PositionedRectangle::PositionedRectangle(Position const& position,
                                         Rectangle const& rectangle)
    : position_(position), rectangle_(rectangle) {}

PositionedRectangle::PositionedRectangle(PositionedRectangle const& other)
    : position_(other.position()), rectangle_(other.rectangle()) {}

PositionedRectangle::PositionedRectangle(PositionedRectangle&& other)
    : position_(move(other.position_)), rectangle_(move(other.rectangle_)) {}

size_t PositionedRectangle::height() const { return rectangle_.height(); }

bool PositionedRectangle::intersectsWith(PositionedEllipse const& other) const {
  return other.contains(Position{x(), y()}) ||
         other.contains(Position{x() + width(), y()}) ||
         other.contains(Position{x(), y() + height()}) ||
         other.contains(Position{x() + width(), y() + height()});
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

Rectangle const& PositionedRectangle::rectangle() const { return rectangle_; }

size_t PositionedRectangle::width() const { return rectangle_.width(); }

size_t PositionedRectangle::x() const { return position_.x(); }

size_t PositionedRectangle::y() const { return position_.y(); }
