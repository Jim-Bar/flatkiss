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
  // FIXME: positionedBoundingBox() too? It seems there are two different things
  // called "bounding boxes".
  PositionedRectangle boundingBox() const;
  bool collidesWith(PositionedSolid const& other) const;
  int64_t height() const;
  /**
   * @brief Returns a new positioned solid moved by the provided displacement.
   *
   * @param vector The displacement to apply.
   * @return PositionedSolid A new positioned solid moved by the vector.
   */
  PositionedSolid operator+(Vector const& vector) const;
  Position const& position() const;
  void position(Position&& new_position);
  Collision const& solid() const;
  int64_t width() const;
  int64_t x() const;
  int64_t y() const;

 private:
  Position position_;
  Collision const solid_;
};

#endif
