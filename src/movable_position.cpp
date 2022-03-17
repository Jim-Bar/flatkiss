#include "movable_position.hpp"

#include <utility>

using std::move;

MovablePosition::MovablePosition(MovablePosition const& other)
    : Position(other) {}

MovablePosition::MovablePosition(MovablePosition&& other) : Position(other) {}

MovablePosition::MovablePosition(Position const& other) : Position(other) {}

MovablePosition::MovablePosition(Position&& other) : Position(other) {}

MovablePosition& MovablePosition::operator=(Position const& other) {
  x_ = other.x();
  y_ = other.y();
  return *this;
}

MovablePosition& MovablePosition::operator=(MovablePosition&& other) {
  x_ = move(other.x_);
  y_ = move(other.y_);
  return *this;
}

void MovablePosition::setX(size_t x) { x_ = x; }

void MovablePosition::setY(size_t y) { y_ = y; }
