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
#include <array>
#include <libflatkiss/logic/navigator.hpp>

using std::abs;
using std::array;
using std::max;
using std::unordered_map;

Navigator::Navigator(unordered_map<int64_t, Solid const> const& solids)
    : solids_{solids} {}

int64_t Navigator::clampToBounds(int64_t object_position, int64_t object_size,
                                 int64_t upper_bound) {
  if (object_position < 0) {
    return 0;
  }

  if (object_position + object_size >= upper_bound) {
    return upper_bound - object_size;
  }

  return object_position;
}

Position Navigator::clampToBounds(PositionedSolid const& positioned_solid,
                                  Level const& level) {
  return Position{
      clampToBounds(positioned_solid.absoluteBoundingBox().x(),
                    positioned_solid.boundingBox().width(),
                    level.widthInTiles() * level.spriteset().spritesWidth()) -
          positioned_solid.boundingBox().x(),
      clampToBounds(positioned_solid.absoluteBoundingBox().y(),
                    positioned_solid.boundingBox().height(),
                    level.heightInTiles() * level.spriteset().spritesHeight()) -
          positioned_solid.boundingBox().y()};
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
    PositionedSolid const& positioned_solid, Position const& destination,
    Level const& level) const {
  /* Decompose the displacement in steps. Each step is a point. Because the
   * components of the displacement can be different, first find the greatest of
   * the two. This is the number of steps. Then move step by step (point by
   * point). On the first collision, return the last position (it did not
   * collide). This is the nearest position to the destination, and which does
   * not collide. Note that this implementation find the nearest position on the
   * line between the source and the destination. It will not return the actual
   * nearest position when it is outside of that line. */
  Vector displacement{destination - positioned_solid.position()};
  int64_t max_displacement{max(abs(displacement.dx()), abs(displacement.dy()))};
  for (int64_t step{1}; step <= max_displacement; step++) {
    Vector partial_displacement{(step * displacement.dx()) / max_displacement,
                                (step * displacement.dy()) / max_displacement};
    if (collidesWithTiles(positioned_solid + partial_displacement, level)) {
      /* Return the last step for which the position does not collide (for the
       * first step this is the original position). */
      return Position{positioned_solid.x() +
                          ((step - 1) * displacement.dx()) / max_displacement,
                      positioned_solid.y() +
                          ((step - 1) * displacement.dy()) / max_displacement};
    }
  }

  /* FIXME: Raise an exception (this must only be called when the destination
   * cannot be reached). */
  return destination;
}

Navigator::MoveResult Navigator::moveBy(PositionedSolid const& positioned_solid,
                                        Vector const& desired_displacement,
                                        Level const& level,
                                        int64_t sidestep_distance,
                                        int64_t sidestep_speed,
                                        bool allow_slide) const {
  /* First collide with the bounds of the level. Compute the resulting
   * (potential) destination. */
  Position destination{
      clampToBounds(positioned_solid + desired_displacement, level)};

  /* Secondly, if the destination is the same as the current position, nothing
   * to do. */
  if (positioned_solid.position() == destination) {
    return {false, positioned_solid.position()};
  }

  // If there is a collision with a tile...
  if (collidesWithTiles(PositionedSolid{destination, positioned_solid.solid()},
                        level)) {
    // Either stick to the tile.
    Position nearest_position{
        findNearestPositionToDestination(positioned_solid, destination, level)};
    if (positioned_solid.position() != nearest_position) {
      return {false, nearest_position};
    }

    // Or slide along it if allowed.
    if (allow_slide) {
      Position slided{slide(positioned_solid, desired_displacement, level)};
      if (slided != positioned_solid.position()) {
        return {false, slided};
      }
    }

    // Or side-step for bypassing it if allowed.
    if (sidestep_distance > 0) {
      Position side_stepped{sideStep(positioned_solid, desired_displacement,
                                     level, sidestep_distance, sidestep_speed)};
      if (side_stepped != positioned_solid.position()) {
        return {true, side_stepped};
      }
    }

    // It is not possible to get to a nearer position.
    return {false, positioned_solid.position()};
  }

  // But if there is no collision, just go to the final destination.
  return {false, destination};
}

/* Side-stepping works by applying the same desired displacement but from a
 * different position. The position is chosen orthogonally to the desired
 * displacement, and according to the side-step lookup distance. This position
 * is called the parallax, it must be a valid position that is not colliding
 * with anything. Then the final parallax position is computed by applying the
 * desired displacement on it. If this final position has some interest (i.e. it
 * is different than the initial parallax position), then the solid is moved
 * toward the initial parallax position. Note that side-stepping is only
 * possible when moving along an axis (not diagonally). */
Position Navigator::sideStep(PositionedSolid const& positioned_solid,
                             Vector const& desired_displacement,
                             Level const& level, int64_t sidestep_distance,
                             int64_t sidestep_speed) const {
  if (desired_displacement.dx() == 0) {
    Position side_stepped{sideStepX(positioned_solid, desired_displacement,
                                    level, sidestep_distance, sidestep_speed)};
    if (side_stepped != positioned_solid.position()) {
      return side_stepped;
    }
  }

  if (desired_displacement.dy() == 0) {
    Position side_stepped{sideStepY(positioned_solid, desired_displacement,
                                    level, sidestep_distance, sidestep_speed)};
    if (side_stepped != positioned_solid.position()) {
      return side_stepped;
    }
  }

  return positioned_solid.position();
}

Position Navigator::sideStepX(PositionedSolid const& positioned_solid,
                              Vector const& desired_displacement,
                              Level const& level, int64_t sidestep_distance,
                              int64_t sidestep_speed) const {
  for (int64_t direction : array{-1, 1}) {
    Position parallax{clampToBounds(
        positioned_solid + Vector{sidestep_distance * direction, 0}, level)};
    if (!collidesWithTiles(PositionedSolid{parallax, positioned_solid.solid()},
                           level)) {
      Position parallax_final{
          moveBy(PositionedSolid{parallax, positioned_solid.solid()},
                 desired_displacement, level, 0, 0, false)
              .position};
      if (parallax_final != parallax) {
        Position side_stepped{positioned_solid.x() + sidestep_speed * direction,
                              positioned_solid.y()};
        if (!collidesWithTiles(
                PositionedSolid{side_stepped, positioned_solid.solid()},
                level)) {
          return side_stepped;
        }
      }
    }
  }

  return positioned_solid.position();
}

Position Navigator::sideStepY(PositionedSolid const& positioned_solid,
                              Vector const& desired_displacement,
                              Level const& level, int64_t sidestep_distance,
                              int64_t sidestep_speed) const {
  for (int64_t direction : array{-1, 1}) {
    Position parallax{clampToBounds(
        positioned_solid + Vector{0, sidestep_distance * direction}, level)};
    if (!collidesWithTiles(PositionedSolid{parallax, positioned_solid.solid()},
                           level)) {
      Position parallax_final{
          moveBy(PositionedSolid{parallax, positioned_solid.solid()},
                 desired_displacement, level, 0, 0, false)
              .position};
      if (parallax_final != parallax) {
        Position side_stepped{
            positioned_solid.x(),
            positioned_solid.y() + sidestep_speed * direction};
        if (!collidesWithTiles(
                PositionedSolid{side_stepped, positioned_solid.solid()},
                level)) {
          return side_stepped;
        }
      }
    }
  }

  return positioned_solid.position();
}

Position Navigator::slide(PositionedSolid const& positioned_solid,
                          Vector const& desired_displacement,
                          Level const& level) const {
  for (Vector const& sliding_displacement : array{
           // Try to slide against the obstacle along the X axis.
           Vector{desired_displacement.dx(), 0},
           // Or slide along the Y axis.
           Vector{0, desired_displacement.dy()},
       }) {
    Position slided{
        moveBy(positioned_solid, sliding_displacement, level, 0, 0, false)
            .position};
    if (slided != positioned_solid.position()) {
      return slided;
    }
  }

  return positioned_solid.position();
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
