#ifndef POSITIONED_SOLID_HPP_INCLUDED
#define POSITIONED_SOLID_HPP_INCLUDED

#include "position.hpp"
#include "solid.hpp"

class PositionedSolid {
 public:
  PositionedSolid(Position const& position, Solid const& solid);
  /**
   * @brief Return the bounding box of the solid moved by the position of the
   * solid.
   *
   * @return PositionedRectangle const& The bounding box.
   */
  PositionedRectangle absoluteBoundingBox() const;
  PositionedRectangle boundingBox() const;
  bool collidesWith(PositionedSolid const& other) const;
  /**
   * @brief Returns a new positioned solid moved by the provided displacement.
   *
   * @param vector The displacement to apply.
   * @return PositionedSolid A new positioned solid moved by the vector.
   */
  PositionedSolid operator+(Vector const& vector) const;
  Position const& position() const;
  void position(Position&& new_position);
  Solid const& solid() const;
  int64_t x() const;
  int64_t y() const;

 private:
  Position position_;
  Solid const solid_;
};

#endif
