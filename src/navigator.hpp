/*
 * Copyright (C) 2021-2022 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
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

#ifndef NAVIGATOR_HPP_INCLUDED
#define NAVIGATOR_HPP_INCLUDED

#include <vector>

#include "collider.hpp"
#include "level.hpp"
#include "positioned_solid.hpp"
#include "tile_solid_mapper.hpp"
#include "vector.hpp"

/**
 * @brief Move a positioned solid to the closest desired location.
 */
class Navigator {
 public:
  Navigator(std::unordered_map<int64_t, Solid const>& solids,
            Level const& level, int64_t tiles_width, int64_t tiles_height);
  Position moveBy(PositionedSolid const& source_positioned_solid,
                  Vector const& desired_displacement) const;

 private:
  Level const& level_;
  std::unordered_map<int64_t, Solid const>& solids_;
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
  bool collidesWithTiles(PositionedSolid const& positioned_solid) const;
  Position findNearestPositionToDestination(
      PositionedSolid const& source_positioned_solid,
      Position const& destination) const;
  bool solidCollidesWithTileAtPosition(PositionedSolid const& positioned_solid,
                                       uint16_t tile_index,
                                       Position const& position) const;
  PositionedSolid solidForTileIndexAtPosition(uint16_t tile_index,
                                              Position const& position) const;
};

#endif
