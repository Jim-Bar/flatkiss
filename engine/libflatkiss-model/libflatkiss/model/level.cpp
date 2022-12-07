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

#include <fstream>
#include <libflatkiss/model/level.hpp>
#include <utility>

using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::streamsize;
using std::string;
using std::unordered_map;
using std::vector;

Level::Level(vector<uint16_t>&& tiles, int64_t width_in_tiles,
             int64_t height_in_tiles, Spriteset const& spriteset,
             AnimationPlayer const& animation_player,
             TileSolidMapper const& tile_solid_mapper)
    : tiles_{move(tiles)},
      width_in_tiles_{width_in_tiles},
      height_in_tiles_{height_in_tiles},
      spriteset_{spriteset},
      animation_player_{animation_player},
      tile_solid_mapper_{tile_solid_mapper} {}

AnimationPlayer const& Level::animationPlayer() const {
  return animation_player_;
}

int64_t Level::heightInTiles() const { return height_in_tiles_; }

Spriteset const& Level::spriteset() const { return spriteset_; }

uint16_t Level::tileIndex(int64_t i, int64_t j) const {
  return tiles_[j * width_in_tiles_ + i];
}

TileSolidMapper const& Level::tileSolidMapper() const {
  return tile_solid_mapper_;
}

int64_t Level::widthInTiles() const { return width_in_tiles_; }

vector<Level> LevelLoader::load(
    string const& file_path, vector<Spriteset> const& spritesets,
    unordered_map<int64_t, AnimationPlayer const>& animation_players,
    unordered_map<int64_t, TileSolidMapper const>& tile_solid_mappers) {
  vector<Level> levels;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      /* TODO: To be consistent with the rest, also store the index of the level
       * in the binary file? */
      uint16_t width_in_tiles{0};
      uint16_t height_in_tiles{0};
      uint16_t spriteset_index{0};
      uint16_t animation_player_index{0};
      uint16_t tile_solid_mapper_index{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&width_in_tiles), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&height_in_tiles), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&spriteset_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&animation_player_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&tile_solid_mapper_index), 2);
      // Two bytes per tile.
      int64_t const size_in_bytes{width_in_tiles * height_in_tiles * 2};
      auto tiles{vector<uint16_t>(size_in_bytes, 0)};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(tiles.data()),
                  static_cast<streamsize>(size_in_bytes));
      levels.emplace_back(move(tiles), width_in_tiles, height_in_tiles,
                          spritesets[spriteset_index],
                          animation_players.at(animation_player_index),
                          tile_solid_mappers.at(tile_solid_mapper_index));
    }
    stream.close();
  }  // FIXME: fail.

  return levels;
}
