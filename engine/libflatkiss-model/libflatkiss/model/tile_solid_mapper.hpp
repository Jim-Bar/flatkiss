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

#ifndef LIBFLATKISS_MODEL_TILE_SOLID_MAPPER_HPP_INCLUDED
#define LIBFLATKISS_MODEL_TILE_SOLID_MAPPER_HPP_INCLUDED

#include <cstdint>
#include <unordered_map>

/**
 * @brief Maps tile indices to solid indices.
 */
class TileSolidMapper {
 public:
  /**
   * @brief Construct a TileSolidMapper from a map of tile indices to matching
   * solid indices.
   *
   * @param tiles_to_solids Map of tile indices to solid indices.
   */
  TileSolidMapper(std::unordered_map<uint16_t, int64_t>&& tiles_to_solids);
  bool contains(uint16_t tile_index) const;
  int64_t solidIndexForTileIndex(uint16_t tile_index) const;

 private:
  std::unordered_map<uint16_t, int64_t> const tiles_to_solids_;
};

#endif
