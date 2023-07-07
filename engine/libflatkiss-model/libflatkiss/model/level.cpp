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

#include <libflatkiss/model/level.hpp>
#include <utility>

using std::move;
using std::vector;

Level::Level(vector<uint16_t>&& tiles, int64_t width_in_tiles,
             int64_t height_in_tiles, Spriteset const& spriteset,
             AnimationPlayer const& animation_player,
             std::vector<Character>& characters,
             TileSolidMapper const& tile_solid_mapper)
    : tiles_{move(tiles)},
      width_in_tiles_{width_in_tiles},
      height_in_tiles_{height_in_tiles},
      spriteset_{spriteset},
      animation_player_{animation_player},
      characters_{move(characters)},
      tile_solid_mapper_{tile_solid_mapper} {}

AnimationPlayer const& Level::animationPlayer() const {
  return animation_player_;
}

vector<Character>& Level::characters() { return characters_; }

int64_t Level::heightInTiles() const { return height_in_tiles_; }

Spriteset const& Level::spriteset() const { return spriteset_; }

uint16_t Level::tileIndex(int64_t i, int64_t j) const {
  return tiles_[j * width_in_tiles_ + i];
}

TileSolidMapper const& Level::tileSolidMapper() const {
  return tile_solid_mapper_;
}

int64_t Level::widthInTiles() const { return width_in_tiles_; }
