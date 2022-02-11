#ifndef NAVIGATOR_HPP_INCLUDED
#define NAVIGATOR_HPP_INCLUDED

#include "collider.hpp"
#include "level.hpp"
#include "positioned_rectangle.hpp"
#include "vector.hpp"

/**
 * @brief Move a positioned rectangle to the closest desired location.
 */
class Navigator {
 public:
  Navigator(Collider const& collider, Level const& level,
            size_t const tiles_size);
  Position moveBy(PositionedRectangle const& source_positioned_rectangle,
                  Vector const& desired_displacement) const;

 private:
  Collider const& collider_;
  Level const& level_;
  size_t const tiles_size_;

  /**
   * @brief Given a position and a movement (delta) on an axis (representing
   * either X or Y), return the resulting position taking account the bounds.
   *
   * @param object_position Position on the axis.
   * @param object_size Size of the object at the given position.
   * @param delta_value Movement of the object.
   * @param upper_bound Maximum position on the axis (the minimum position is
   * zero).
   * @return size_t The resulting position of the object on the axis after
   * applying the movement, and clamping to the bounds.
   */
  size_t clampToBounds(size_t object_position, size_t object_size,
                       int64_t delta_value, size_t upper_bound) const;
  // FIXME: Move to Collider?
  bool collidesWithTiles(PositionedRectangle const& positioned_rectangle) const;
  Position findNearestPositionToDestination(
      PositionedRectangle const& source_positioned_rectangle,
      Position const& destination) const;
};

#endif
