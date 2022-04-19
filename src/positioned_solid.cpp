#include "positioned_solid.hpp"

PositionedSolid::PositionedSolid(Position const& position,
                                 Collision const& solid)
    : position_{position}, solid_{solid} {}

PositionedRectangle PositionedSolid::boundingBox() const {
  return position_ + solid_.boundingBox();
}

bool PositionedSolid::collidesWith(PositionedSolid const& other) const {
  // FIXME: Can auto be used (reference)?
  for (PositionedEllipse const& pos_ell : solid_.positionedEllipses()) {
    for (PositionedEllipse const& other_pos_ell :
         other.solid_.positionedEllipses()) {
      /* TODO: implement.
      if ((position_ + pos_ell)
              .intersectsWith(other.position_ + other_pos_ell)) {
        return true;
      } */
    }
    for (PositionedRectangle const& other_pos_rect :
         other.solid_.positionedRectangles()) {
      if ((other.position_ + other_pos_rect)
              .intersectsWith(position_ + pos_ell)) {
        return true;
      }
    }
  }

  // FIXME: Can auto be used (reference)?
  for (PositionedRectangle const& pos_rect : solid_.positionedRectangles()) {
    for (PositionedEllipse const& other_pos_ell :
         other.solid_.positionedEllipses()) {
      if ((position_ + pos_rect)
              .intersectsWith(other.position_ + other_pos_ell)) {
        return true;
      }
    }
    for (PositionedRectangle const& other_pos_rect :
         other.solid_.positionedRectangles()) {
      if ((position_ + pos_rect)
              .intersectsWith(other.position_ + other_pos_rect)) {
        return true;
      }
    }
  }

  return false;
}

Position const& PositionedSolid::position() const { return position_; }

Collision const& PositionedSolid::solid() const { return solid_; }
