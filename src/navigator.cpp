#include "navigator.hpp"

#include <algorithm>

using std::abs;
using std::max;
using std::unordered_map;

Navigator::Navigator(TileSolidMapper const& tile_solid_mapper,
                     unordered_map<int64_t, Solid const>& solids,
                     Level const& level, int64_t tiles_width,
                     int64_t tiles_height)
    : tile_solid_mapper_{tile_solid_mapper},
      solids_{solids},
      level_{level},
      tiles_width_{tiles_width},
      tiles_height_{tiles_height} {}

int64_t Navigator::clampToBounds(int64_t object_position, int64_t object_size,
                                 int64_t delta_value, int64_t upper_bound) {
  if (object_position + delta_value < 0) {
    return 0;
  }

  if (object_position + object_size + delta_value >= upper_bound) {
    return upper_bound - object_size;
  }

  return object_position + delta_value;
}

bool Navigator::collidesWithTiles(
    PositionedSolid const& positioned_solid) const {
  for (int64_t y(positioned_solid.absoluteBoundingBox().y() / tiles_height_);
       y <= (positioned_solid.absoluteBoundingBox().y() +
             positioned_solid.boundingBox().height() - 1) /
                tiles_height_;
       y++) {
    for (int64_t x(positioned_solid.absoluteBoundingBox().x() / tiles_width_);
         x <= (positioned_solid.absoluteBoundingBox().x() +
               positioned_solid.boundingBox().width() - 1) /
                  tiles_width_;
         x++) {
      uint16_t tile_index(level_.tileIndex(x, y));
      if (solidCollidesWithTileAtPosition(
              positioned_solid, tile_index,
              Position{x * tiles_width_, y * tiles_height_})) {
        return true;
      }
    }
  }

  return false;
}

Position Navigator::findNearestPositionToDestination(
    PositionedSolid const& source_positioned_solid,
    Position const& destination) const {
  /* Decompose the displacement in steps. Each step is a point. Because the
   * components of the displacement can be different, first find the greatest of
   * the two. This is the number of steps. Then move step by step (point by
   * point). On the first collision, return the last position (it did not
   * collide). This is the nearest position to the destination, and which does
   * not collide. Note that this implementation find the nearest position on the
   * line between the source and the destination. It will not return the actual
   * nearest position when it is outside of that line. */
  Vector displacement{destination - source_positioned_solid.position()};
  int64_t max_displacement{max(abs(displacement.dx()), abs(displacement.dy()))};
  for (int64_t step{1}; step <= max_displacement; step++) {
    Vector partial_displacement{(step * displacement.dx()) / max_displacement,
                                (step * displacement.dy()) / max_displacement};
    if (collidesWithTiles(source_positioned_solid + partial_displacement)) {
      /* Return the last step for which the position does not collide (for the
       * first step this is the original position). */
      return Position{source_positioned_solid.x() +
                          ((step - 1) * displacement.dx()) / max_displacement,
                      source_positioned_solid.y() +
                          ((step - 1) * displacement.dy()) / max_displacement};
    }
  }

  /* FIXME: Raise an exception (this must only be called when the destination
   * cannot be reached). */
  return destination;
}

Position Navigator::moveBy(PositionedSolid const& source_positioned_solid,
                           Vector const& desired_displacement) const {
  /* First collide with the bounds of the level. Compute the resulting
   * (potential) destination. */
  Position destination{
      clampToBounds(source_positioned_solid.absoluteBoundingBox().x(),
                    source_positioned_solid.boundingBox().width(),
                    desired_displacement.dx(),
                    level_.widthInTiles() * tiles_width_) -
          source_positioned_solid.boundingBox().x(),
      clampToBounds(source_positioned_solid.absoluteBoundingBox().y(),
                    source_positioned_solid.boundingBox().height(),
                    desired_displacement.dy(),
                    level_.heightInTiles() * tiles_height_) -
          source_positioned_solid.boundingBox().y()};

  /* Secondly, if the destination is the same as the current position, nothing
   * to do. */
  if (source_positioned_solid.position() == destination) {
    return source_positioned_solid.position();
  }

  /* Otherwise if there is a collision with a tile, make sure to stick to the
   * tile. */
  if (collidesWithTiles(
          PositionedSolid{destination, source_positioned_solid.solid()})) {
    Position nearest_position{
        findNearestPositionToDestination(source_positioned_solid, destination)};
    if (source_positioned_solid.position() != nearest_position) {
      return nearest_position;
    }

    // Maybe it is possible to slide against the obstacle along the X axis.
    Position destination_x{destination.x(), source_positioned_solid.y()};
    if (collidesWithTiles(
            PositionedSolid{destination_x, source_positioned_solid.solid()})) {
      Position nearest_position_x{findNearestPositionToDestination(
          source_positioned_solid, destination_x)};
      if (source_positioned_solid.position() != nearest_position_x) {
        return nearest_position_x;
      }
    } else {
      return destination_x;
    }

    // Or along the Y axis.
    Position destination_y{source_positioned_solid.x(), destination.y()};
    if (collidesWithTiles(
            PositionedSolid{destination_y, source_positioned_solid.solid()})) {
      Position nearest_position_y{findNearestPositionToDestination(
          source_positioned_solid, destination_y)};
      if (source_positioned_solid.position() != nearest_position_y) {
        return nearest_position_y;
      }
    } else {
      return destination_y;
    }

    // It is not possible to get to a nearer position.
    return source_positioned_solid.position();
  }

  // But if there is not collision, just go to the final destination.
  return destination;
}

bool Navigator::solidCollidesWithTileAtPosition(
    PositionedSolid const& positioned_solid, uint16_t tile_index,
    Position const& position) const {
  if (tile_solid_mapper_.contains(tile_index)) {
    return positioned_solid.collidesWith(
        solidForTileIndexAtPosition(tile_index, position));
  }

  return false;
}

PositionedSolid Navigator::solidForTileIndexAtPosition(
    uint16_t tile_index, Position const& position) const {
  return PositionedSolid{
      position,
      solids_.at(tile_solid_mapper_.solidIndexForTileIndex(tile_index))};
}
