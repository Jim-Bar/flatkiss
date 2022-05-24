#ifndef POSITIONED_SOLID_HPP_INCLUDED
#define POSITIONED_SOLID_HPP_INCLUDED

#include <vector>

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
  /**
   * @brief Returns a new positioned solid moved by the provided displacement.
   *
   * @param vector The displacement to apply.
   * @return PositionedSolid A new positioned solid moved by the vector.
   */
  PositionedSolid operator+(Vector const& vector) const;
  Position const& position() const;
  void position(Position&& new_position);
  std::vector<PositionedEllipse> const& positionedEllipses() const;
  std::vector<PositionedRectangle> const& positionedRectangles() const;
  Solid const& solid() const;
  int64_t x() const;
  int64_t y() const;

 private:
  Position position_;
  std::vector<PositionedEllipse> const positioned_ellipses_;
  std::vector<PositionedRectangle> const positioned_rectangles_;
  Solid const solid_;

  static std::vector<PositionedEllipse> create_positioned_ellipses(
      Position const& position, Solid const& solid);
  static std::vector<PositionedRectangle> create_positioned_rectangles(
      Position const& position, Solid const& solid);
};

#endif
