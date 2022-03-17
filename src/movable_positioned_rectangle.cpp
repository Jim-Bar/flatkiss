#include "movable_positioned_rectangle.hpp"

#include <utility>

using std::move;

MovablePositionedRectangle::MovablePositionedRectangle(
    Position const& position, Rectangle const& rectangle)
    : PositionedRectangle(position, rectangle) {}

void MovablePositionedRectangle::moveTo(Position const& position) {
  position_ = position;
}

void MovablePositionedRectangle::moveTo(Position&& position) {
  position_ = move(position);
}

void MovablePositionedRectangle::setX(size_t x) { position_.setX(x); }

void MovablePositionedRectangle::setY(size_t y) { position_.setY(y); }
