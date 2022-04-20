#ifndef NAVIGATOR_HPP_INCLUDED
#define NAVIGATOR_HPP_INCLUDED

#include "collider.hpp"
#include "level.hpp"
#include "positioned_solid.hpp"
#include "vector.hpp"

/**
 * @brief Move a positioned solid to the closest desired location.
 */
class Navigator {
 public:
  Navigator(Collider const& collider, Level const& level, int64_t tiles_width,
            int64_t tiles_height);
  Position moveBy(PositionedSolid const& source_positioned_solid,
                  Vector const& desired_displacement) const;

 private:
  Collider const& collider_;
  Level const& level_;
  int64_t const tiles_height_;
  int64_t const tiles_width_;

  /**
   * @brief Given a position and a movement (delta) on an axis (representing
   * either X or Y), return the resulting position taking account the bounds.
   *
   * @param object_position Position on the axis.
   * @param object_size Size of the object at the given position.
   * @param delta_value Movement of the object.
   * @param upper_bound Maximum position on the axis (the minimum position is
   * zero).
   * @return int64_t The resulting position of the object on the axis after
   * applying the movement, and clamping to the bounds.
   */
  static int64_t clampToBounds(int64_t object_position, int64_t object_size,
                               int64_t delta_value, int64_t upper_bound);
  // FIXME: Move to Collider?
  bool collidesWithTiles(PositionedSolid const& positioned_solid) const;
  Position findNearestPositionToDestination(
      PositionedSolid const& source_positioned_solid,
      Position const& destination) const;
};

#endif
