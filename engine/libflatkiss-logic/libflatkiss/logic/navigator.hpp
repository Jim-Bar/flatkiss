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

#ifndef LIBFLATKISS_LOGIC_NAVIGATOR_HPP_INCLUDED
#define LIBFLATKISS_LOGIC_NAVIGATOR_HPP_INCLUDED

#include <libflatkiss/logic/collider.hpp>
#include <libflatkiss/model/model.hpp>
#include <vector>

/**
 * @brief Move a positioned solid to the closest desired location.
 */
class Navigator {
 public:
  struct MoveResult {
    bool const has_side_stepped;
    Position position;
  };

  Navigator(std::unordered_map<int64_t, Solid const> const& solids);
  /**
   * @brief Try to move a solid according to the provided movement.
   *
   * If the movement would cause the solid to enter an obstacle, then the method
   * returns a position which sticks the solid to the obstacle.
   *
   * The method can cause the solid to slide against obstacles: for instance
   * when going to the bottom left but there is a wall to the bottom, then slide
   * to the left along the wall.
   *
   * The method can also cause the solid to side-step, that it bypassing
   * obstacles. For example if the solid is blocked by an object, but is also
   * near the edge of the object, then moving a little bit (side-stepping) could
   * allow the solid to pass. The solid lookups according to a distance, but
   * will only move by a fraction of that distance. Calling the method again
   * several times will move the solid bit by bit until, finally, the edge of
   * the object is reached and bypassed (side-stepped).
   *
   * @param positioned_solid Solid to try to move.
   * @param desired_displacement Movement applied to the solid.
   * @param level Level in which the solid is moving.
   * @param sidestep_distance How far the solid looks for side-stepping.
   * @param sidestep_speed How much the solid actually side-steps in one step.
   * @param allow_slide Whether to enable sliding.
   */
  MoveResult moveBy(PositionedSolid const& positioned_solid,
                    Vector const& desired_displacement, Level const& level,
                    int64_t sidestep_distance, int64_t sidestep_speed,
                    bool allow_slide) const;

 private:
  std::unordered_map<int64_t, Solid const> const& solids_;

  /**
   * @brief Given a position on an axis (representing either X or Y), return the
   * resulting position taking account the bounds.
   *
   * @param object_position Position on the axis.
   * @param object_size Size of the object at the given position.
   * @param upper_bound Maximum position on the axis (the minimum position is
   * zero).
   * @return int64_t The resulting position of the object, clamping to the
   * bounds.
   */
  static int64_t clampToBounds(int64_t object_position, int64_t object_size,
                               int64_t upper_bound);
  static Position clampToBounds(PositionedSolid const& positioned_solid,
                                Level const& level);
  bool collidesWithTiles(PositionedSolid const& positioned_solid,
                         Level const& level) const;
  Position findNearestPositionToDestination(
      PositionedSolid const& positioned_solid, Position const& destination,
      Level const& level) const;
  Position sideStep(PositionedSolid const& positioned_solid,
                    Vector const& desired_displacement, Level const& level,
                    int64_t sidestep_distance, int64_t sidestep_speed) const;
  Position sideStepX(PositionedSolid const& positioned_solid,
                     Vector const& desired_displacement, Level const& level,
                     int64_t sidestep_distance, int64_t sidestep_speed) const;
  Position sideStepY(PositionedSolid const& positioned_solid,
                     Vector const& desired_displacement, Level const& level,
                     int64_t sidestep_distance, int64_t sidestep_speed) const;
  Position slide(PositionedSolid const& positioned_solid,
                 Vector const& desired_displacement, Level const& level) const;
  bool solidCollidesWithTileAtPosition(PositionedSolid const& positioned_solid,
                                       uint16_t tile_index,
                                       Position const& position,
                                       Level const& level) const;
  PositionedSolid solidForTileIndexAtPosition(uint16_t tile_index,
                                              Position const& position,
                                              Level const& level) const;
};

#endif
