/*
 * Copyright (C) 2021-2023 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Refer to 'COPYING.txt' for the full notice.
 */

#include <algorithm>
#include <libflatkiss/logic/navigator.hpp>

using std::abs;
using std::max;
using std::unordered_map;

Navigator::Navigator(unordered_map<int64_t, Solid const> const& solids)
    : solids_{solids} {}

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

bool Navigator::collidesWithTiles(PositionedSolid const& positioned_solid,
                                  Level const& level) const {
  for (int64_t y(positioned_solid.absoluteBoundingBox().y() /
                 level.spriteset().spritesHeight());
       y <= (positioned_solid.absoluteBoundingBox().y() +
             positioned_solid.boundingBox().height() - 1) /
                level.spriteset().spritesHeight();
       y++) {
    for (int64_t x(positioned_solid.absoluteBoundingBox().x() /
                   level.spriteset().spritesWidth());
         x <= (positioned_solid.absoluteBoundingBox().x() +
               positioned_solid.boundingBox().width() - 1) /
                  level.spriteset().spritesWidth();
         x++) {
      uint16_t tile_index(level.tileIndex(x, y));
      if (solidCollidesWithTileAtPosition(
              positioned_solid, tile_index,
              Position{x * level.spriteset().spritesWidth(),
                       y * level.spriteset().spritesHeight()},
              level)) {
        return true;
      }
    }
  }

  return false;
}

Position Navigator::findNearestPositionToDestination(
    PositionedSolid const& source_positioned_solid, Position const& destination,
    Level const& level) const {
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
    if (collidesWithTiles(source_positioned_solid + partial_displacement,
                          level)) {
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
                           Vector const& desired_displacement,
                           Level const& level) const {
  /* First collide with the bounds of the level. Compute the resulting
   * (potential) destination. */
  Position destination{
      clampToBounds(source_positioned_solid.absoluteBoundingBox().x(),
                    source_positioned_solid.boundingBox().width(),
                    desired_displacement.dx(),
                    level.widthInTiles() * level.spriteset().spritesWidth()) -
          source_positioned_solid.boundingBox().x(),
      clampToBounds(source_positioned_solid.absoluteBoundingBox().y(),
                    source_positioned_solid.boundingBox().height(),
                    desired_displacement.dy(),
                    level.heightInTiles() * level.spriteset().spritesHeight()) -
          source_positioned_solid.boundingBox().y()};

  /* Secondly, if the destination is the same as the current position, nothing
   * to do. */
  if (source_positioned_solid.position() == destination) {
    return source_positioned_solid.position();
  }

  /* Otherwise if there is a collision with a tile, make sure to stick to the
   * tile. */
  if (collidesWithTiles(
          PositionedSolid{destination, source_positioned_solid.solid()},
          level)) {
    Position nearest_position{findNearestPositionToDestination(
        source_positioned_solid, destination, level)};
    if (source_positioned_solid.position() != nearest_position) {
      return nearest_position;
    }

    /* FIXME: Bypass the obstacle further. A possible solution would be to
     * recursively invoke moveBy() with varying displacements. */
    for (Position const& blah : std::array{
             // Maybe sliding against the obstacle along the X axis is possible.
             Position{destination.x(), source_positioned_solid.y()},
             // Or slide along the Y axis.
             Position{source_positioned_solid.x(), destination.y()},
             // Otherwise bypass by incrementing along the X axis.
             Position{destination.x() + 1, destination.y()},
             // Or bypass by decrementing along the X axis.
             Position{destination.x() - 1, destination.y()},
             // Or bypass by incrementing along the Y axis.
             Position{destination.x(), destination.y() + 1},
             // Or bypass by decrementing along the Y axis.
             Position{destination.x(), destination.y() - 1}}) {
      if (blah != source_positioned_solid.position()) {
        if (collidesWithTiles(
                PositionedSolid{blah, source_positioned_solid.solid()},
                level)) {
          Position nearest_position_x{findNearestPositionToDestination(
              source_positioned_solid, blah, level)};
          if (source_positioned_solid.position() != nearest_position_x) {
            return nearest_position_x;
          }
        } else {
          return blah;
        }
      }
    }

    // It is not possible to get to a nearer position.
    return source_positioned_solid.position();
  }

  // But if there is no collision, just go to the final destination.
  return destination;
}

bool Navigator::solidCollidesWithTileAtPosition(
    PositionedSolid const& positioned_solid, uint16_t tile_index,
    Position const& position, Level const& level) const {
  if (level.tileSolidMapper().contains(tile_index)) {
    return Collider::collide(
        positioned_solid,
        solidForTileIndexAtPosition(tile_index, position, level));
  }

  return false;
}

PositionedSolid Navigator::solidForTileIndexAtPosition(
    uint16_t tile_index, Position const& position, Level const& level) const {
  return PositionedSolid{
      position,
      solids_.at(level.tileSolidMapper().solidIndexForTileIndex(tile_index))};
}
