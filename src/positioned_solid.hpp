#ifndef SOLID_HPP_INCLUDED
#define SOLID_HPP_INCLUDED

#include "collision.hpp"
#include "position.hpp"

class PositionedSolid {
 public:
  PositionedSolid(Position const& position, Collision const& solid);
  /**
   * @brief Return the bounding box of the solid moved by the position of the
   * solid.
   *
   * @return PositionedRectangle const& The bounding box.
   */
  PositionedRectangle boundingBox() const;
  bool collidesWith(PositionedSolid const& other) const;
  Position const& position() const;
  Collision const& solid() const;

 private:
  Position position_;
  Collision const solid_;
};

#endif
