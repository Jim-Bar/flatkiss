#ifndef COLLISION_HPP_INCLUDED
#define COLLISION_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>

#include "positioned_ellipse.hpp"
#include "positioned_rectangle.hpp"

// FIXME: Rename to `Solid`.
/**
 * @brief An area made up of several rectangles and which cannot be crossed.
 */
class Collision {
 public:
  Collision(std::vector<PositionedEllipse> positioned_ellipses,
            std::vector<PositionedRectangle> positioned_rectangles);
  /**
   * @brief Return the minimal rectangle containing all the shapes and
   * positioned inside the collision at the location (x, y) defined by the
   * smallest x-position (respectively y-position) among all the shapes.
   *
   * @return PositionedRectangle const& The bounding box.
   */
  PositionedRectangle const& boundingBox() const;
  // TODO: Use an abstract class PositionedShape?
  std::vector<PositionedEllipse> const& positionedEllipses() const;
  std::vector<PositionedRectangle> const& positionedRectangles() const;

 private:
  PositionedRectangle const bounding_box_;
  std::vector<PositionedEllipse> const positioned_ellipses_;
  std::vector<PositionedRectangle> const positioned_rectangles_;

  static PositionedRectangle computeBoundingBox(
      std::vector<PositionedEllipse> positioned_ellipses,
      std::vector<PositionedRectangle> positioned_rectangles);
};

#endif
