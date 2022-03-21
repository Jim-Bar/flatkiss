#include "navigator.hpp"

#include <algorithm>

using std::abs;
using std::max;

Navigator::Navigator(Collider const& collider, Level const& level,
                     size_t tiles_size)
    : collider_{collider}, level_{level}, tiles_size_{tiles_size} {}

size_t Navigator::clampToBounds(size_t object_position, size_t object_size,
                                int64_t delta_value, size_t upper_bound) {
  // Here care is taken to deal with unsigned integers and substractions.
  if (delta_value < 0 && object_position < -delta_value) {
    return 0;
  }

  if (delta_value > 0 &&
      object_position + object_size + delta_value >= upper_bound) {
    return upper_bound - object_size;
  }

  return object_position + delta_value;
}

bool Navigator::collidesWithTiles(
    PositionedRectangle const& positioned_rectangle) const {
  for (size_t y(positioned_rectangle.y() / tiles_size_);
       y <= (positioned_rectangle.y() + positioned_rectangle.height() - 1) /
                tiles_size_;
       y++) {
    for (size_t x(positioned_rectangle.x() / tiles_size_);
         x <= (positioned_rectangle.x() + positioned_rectangle.width() - 1) /
                  tiles_size_;
         x++) {
      uint16_t tile_index(level_.tileIndex(x, y));
      if (collider_.collide(positioned_rectangle, tile_index,
                            Position{x * tiles_size_, y * tiles_size_})) {
        return true;
      }
    }
  }

  return false;
}

Position Navigator::findNearestPositionToDestination(
    PositionedRectangle const& source_positioned_rectangle,
    Position const& destination) const {
  /* Decompose the displacement in steps. Each step is a point. Because the
   * components of the displacement can be different, first find the greatest of
   * the two. This is the number of steps. Then move step by step (point by
   * point). On the first collision, return the last position (it did not
   * collide). This is the nearest position to the destination, and which does
   * not collide. Note that this implementation find the nearest position on the
   * line between the source and the destination. It will not return the actual
   * nearest position when it is outside of that line. */
  Vector displacement{destination - source_positioned_rectangle.position()};
  int64_t max_displacement{max(abs(displacement.dx()), abs(displacement.dy()))};
  for (int64_t step{1}; step <= max_displacement; step++) {
    Vector partial_displacement{(step * displacement.dx()) / max_displacement,
                                (step * displacement.dy()) / max_displacement};
    if (collidesWithTiles(source_positioned_rectangle + partial_displacement)) {
      /* Return the last step for which the position does not collide (for the
       * first step this is the original position). */
      return Position{source_positioned_rectangle.x() +
                          ((step - 1) * displacement.dx()) / max_displacement,
                      source_positioned_rectangle.y() +
                          ((step - 1) * displacement.dy()) / max_displacement};
    }
  }

  /* FIXME: Raise an exception (this must only be called when the destination
   * cannot be reached). */
  return destination;
}

Position Navigator::moveBy(
    PositionedRectangle const& source_positioned_rectangle,
    Vector const& desired_displacement) const {
  /* First collide with the bounds of the level. Compute the resulting
   * (potential) destination. */
  Position destination{
      clampToBounds(
          source_positioned_rectangle.x(), source_positioned_rectangle.width(),
          desired_displacement.dx(), level_.widthInTiles() * tiles_size_),
      clampToBounds(
          source_positioned_rectangle.y(), source_positioned_rectangle.height(),
          desired_displacement.dy(), level_.heightInTiles() * tiles_size_)};

  /* Secondly, if the destination is the same as the current position, nothing
   * to do. */
  if (source_positioned_rectangle.position() == destination) {
    return source_positioned_rectangle.position();
  }

  /* Otherwise if there is a collision with a tile, make sure to stick to the
   * tile. */
  if (collidesWithTiles(PositionedRectangle{
          destination, source_positioned_rectangle.rectangle()})) {
    Position nearest_position{findNearestPositionToDestination(
        source_positioned_rectangle, destination)};
    if (source_positioned_rectangle.position() != nearest_position) {
      return nearest_position;
    }

    // Maybe it is possible to slide against the obstacle along the X axis.
    Position destination_x{destination.x(), source_positioned_rectangle.y()};
    if (collidesWithTiles(PositionedRectangle{
            destination_x, source_positioned_rectangle.rectangle()})) {
      Position nearest_position_x{findNearestPositionToDestination(
          source_positioned_rectangle, destination_x)};
      if (source_positioned_rectangle.position() != nearest_position_x) {
        return nearest_position_x;
      }
    } else {
      return destination_x;
    }

    // Or along the Y axis.
    Position destination_y{source_positioned_rectangle.x(), destination.y()};
    if (collidesWithTiles(PositionedRectangle{
            destination_y, source_positioned_rectangle.rectangle()})) {
      Position nearest_position_y{findNearestPositionToDestination(
          source_positioned_rectangle, destination_y)};
      if (source_positioned_rectangle.position() != nearest_position_y) {
        return nearest_position_y;
      }
    } else {
      return destination_y;
    }

    // It is not possible to get to a nearer position.
    return source_positioned_rectangle.position();
  }

  // But if there is not collision, just go to the final destination.
  return destination;
}
