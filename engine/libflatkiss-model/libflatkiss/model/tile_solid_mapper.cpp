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

#include <libflatkiss/model/tile_solid_mapper.hpp>
#include <utility>

using std::move;
using std::unordered_map;

TileSolidMapper::TileSolidMapper(
    unordered_map<uint16_t, int64_t>&& tiles_to_solids)
    : tiles_to_solids_{move(tiles_to_solids)} {}

bool TileSolidMapper::contains(uint16_t tile_index) const {
  return tiles_to_solids_.contains(tile_index);
}

int64_t TileSolidMapper::solidIndexForTileIndex(uint16_t tile_index) const {
  if (tiles_to_solids_.contains(tile_index)) {
    return tiles_to_solids_.at(tile_index);
  }

  return 0;  // FIXME: Raise exception.
}
