#include "positioned_solid.hpp"

#include <utility>

#include "collider.hpp"

using std::move;

PositionedSolid::PositionedSolid(Position const& position, Solid const& solid)
    : position_{position}, solid_{solid} {}

PositionedRectangle PositionedSolid::absoluteBoundingBox() const {
  return position_ + solid_.boundingBox();
}

PositionedRectangle PositionedSolid::boundingBox() const {
  return solid_.boundingBox();
}

bool PositionedSolid::collidesWith(PositionedSolid const& other) const {
  for (auto const& pos_ell : solid_.positionedEllipses()) {
    for (auto const& other_pos_ell : other.solid_.positionedEllipses()) {
      if (Collider::collide(position_ + pos_ell, other.position_ + other_pos_ell)) {
        return true;
      }
    }
    for (auto const& other_pos_rect : other.solid_.positionedRectangles()) {
      if (Collider::collide(other.position_ + other_pos_rect, position_ + pos_ell)) {
        return true;
      }
    }
  }

  for (auto const& pos_rect : solid_.positionedRectangles()) {
    for (auto const& other_pos_ell : other.solid_.positionedEllipses()) {
      if (Collider::collide(position_ + pos_rect, other.position_ + other_pos_ell)) {
        return true;
      }
    }
    for (auto const& other_pos_rect : other.solid_.positionedRectangles()) {
      if (Collider::collide(position_ + pos_rect, other.position_ + other_pos_rect)) {
        return true;
      }
    }
  }

  return false;
}

PositionedSolid PositionedSolid::operator+(Vector const& vector) const {
  return PositionedSolid{position() + vector, solid_};
}

Position const& PositionedSolid::position() const { return position_; }

void PositionedSolid::position(Position&& new_position) {
  position_ = move(new_position);
}

Solid const& PositionedSolid::solid() const { return solid_; }

int64_t PositionedSolid::x() const { return position_.x(); }

int64_t PositionedSolid::y() const { return position_.y(); }
