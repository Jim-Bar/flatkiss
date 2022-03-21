#include "position.hpp"

#include <utility>

#include "positioned_ellipse.hpp"
#include "positioned_rectangle.hpp"
#include "rectangle.hpp"

using std::move;

Position::Position(size_t X, size_t Y) : x_{X}, y_{Y} {}

Position::Position(Position const& other) : x_{other.x()}, y_{other.y()} {}

Position::Position(Position&& other) : x_{move(other.x_)}, y_{move(other.y_)} {}

Position::~Position() {}

Position& Position::operator=(Position&& other) {
  x_ = move(other.x_);
  y_ = move(other.y_);
  return *this;
}

bool Position::operator!=(Position const& other) const {
  return !(*this == other);
}

Position Position::operator+(Position const& other) const {
  return Position{x() + other.x(), y() + other.y()};
}

PositionedEllipse Position::operator+(PositionedEllipse const& ellipse) const {
  return PositionedEllipse{*this + ellipse.position(), ellipse.ellipse()};
}

PositionedRectangle Position::operator+(
    PositionedRectangle const& rectangle) const {
  return PositionedRectangle{*this + rectangle.position(),
                             rectangle.rectangle()};
}

Position Position::operator+(Vector const& vector) const {
  return Position{x() + vector.dx(), y() + vector.dy()};
}

Vector Position::operator-(Position const& other) const {
  return Vector{substractSizes(x(), other.x()), substractSizes(y(), other.y())};
}

bool Position::operator==(Position const& other) const {
  return x() == other.x() && y() == other.y();
}

int64_t Position::substractSizes(size_t size_1, size_t size_2) {
  if (size_1 >= size_2) {
    return static_cast<int64_t>(size_1 - size_2);
  }

  return -static_cast<int64_t>(size_2 - size_1);
}

size_t Position::x() const { return x_; }

void Position::x(size_t newX) { x_ = newX; }

size_t Position::y() const { return y_; }

void Position::y(size_t newY) { y_ = newY; }
