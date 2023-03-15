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
  /* Disabling lint for short variables names because they are useful for
   * math-related things (x, y, ...). */
  // NOLINTBEGIN(readability-identifier-length)
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
  // NOLINTEND(readability-identifier-length)

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

Navigator::MoveResult Navigator::moveBy(
    PositionedSolid const& source_positioned_solid,
    Vector const& desired_displacement, Level const& level,
    int64_t sidestep_distance, bool slide) const {
  /* First collide with the bounds of the level. Compute the resulting
   * (potential) destination. */
  Position destination{
      clampToBounds(source_positioned_solid.absoluteBoundingBox().x(),
                    source_positioned_solid.boundingBox().width(),
                    desired_displacement.dx(),
                    level.widthInTiles() * level.spriteset().spritesWidth()) -
          source_positioned_solid.boundingBox().x(),  // FIXME: - x + w?
      clampToBounds(source_positioned_solid.absoluteBoundingBox().y(),
                    source_positioned_solid.boundingBox().height(),
                    desired_displacement.dy(),
                    level.heightInTiles() * level.spriteset().spritesHeight()) -
          source_positioned_solid.boundingBox().y()};  // FIXME: - y + h?

  /* Secondly, if the destination is the same as the current position, nothing
   * to do. */
  if (source_positioned_solid.position() == destination) {
    return {false, source_positioned_solid.position()};
  }

  /* Otherwise if there is a collision with a tile, make sure to stick to the
   * tile. */
  if (collidesWithTiles(
          PositionedSolid{destination, source_positioned_solid.solid()},
          level)) {
    Position nearest_position{findNearestPositionToDestination(
        source_positioned_solid, destination, level)};
    if (source_positioned_solid.position() != nearest_position) {
      return {false, nearest_position};
    }

    if (slide) {
      for (Vector const& sliding_displacement : std::array{
               // Try to slide against the obstacle along the X axis.
               Vector{desired_displacement.dx(), 0},
               // Or slide along the Y axis.
               Vector{0, desired_displacement.dy()},
           }) {
        Position blah{moveBy(source_positioned_solid, sliding_displacement,
                             level, 0, false)
                          .position};
        if (blah != source_positioned_solid.position()) {
          return {false, blah};
        }
      }
    }

    /* Side-stepping works by applying the same desired displacement but from a
     * different position. The position is chosen orthogonally to the desired
     * displacement, and according to the side-step lookup distance. This
     * position is called the parallax, it must be a valid position that is not
     * colliding with anything. Then the final parallax position is computed by
     * applying the desired displacement on it. If this final position has some
     * interest (i.e. it is different than the initial parallax position), then
     * the solid is moved toward the initial parallax position. Note that
     * side-stepping is only possible when moving along an axis (not
     * diagonally). */
    if (sidestep_distance > 0) {
      if (desired_displacement.dx() == 0) {
        for (int64_t direction : std::array{-1, 1}) {
          // FIXME: Must be clamped to bounds.
          Position parallax{
              source_positioned_solid.x() + sidestep_distance * direction,
              source_positioned_solid.y()};
          if (!collidesWithTiles(
                  PositionedSolid{parallax, source_positioned_solid.solid()},
                  level)) {
            Position parallax_final{
                moveBy(
                    PositionedSolid{parallax, source_positioned_solid.solid()},
                    desired_displacement, level, 0, false)
                    .position};
            if (parallax_final != parallax) {
              // FIXME: Multiply by speed in pixels.
              Position side_stepped{source_positioned_solid.x() + direction,
                                    source_positioned_solid.y()};
              if (!collidesWithTiles(
                      PositionedSolid{side_stepped,
                                      source_positioned_solid.solid()},
                      level)) {
                return {true, side_stepped};
              }
            }
          }
        }
      }

      if (desired_displacement.dy() == 0) {
        for (int64_t direction : std::array{-1, 1}) {
          // FIXME: Must be clamped to bounds.
          Position parallax{
              source_positioned_solid.x(),
              source_positioned_solid.y() + sidestep_distance * direction};
          if (!collidesWithTiles(
                  PositionedSolid{parallax, source_positioned_solid.solid()},
                  level)) {
            Position parallax_final{
                moveBy(
                    PositionedSolid{parallax, source_positioned_solid.solid()},
                    desired_displacement, level, 0, false)
                    .position};
            if (parallax_final != parallax) {
              Position side_stepped{source_positioned_solid.x(),
                                    // FIXME: Multiply by speed in pixels.
                                    source_positioned_solid.y() + direction};
              if (!collidesWithTiles(
                      PositionedSolid{side_stepped,
                                      source_positioned_solid.solid()},
                      level)) {
                return {true, side_stepped};
              }
            }
          }
        }
      }
    }

    // It is not possible to get to a nearer position.
    return {false, source_positioned_solid.position()};
  }

  // But if there is no collision, just go to the final destination.
  return {false, destination};
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
