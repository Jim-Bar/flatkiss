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
#include <libflatkiss/data/loader_level.hpp>
#include <libflatkiss/data/stream_reader.hpp>
#include <libflatkiss/model/level.hpp>

using std::ifstream;
using std::ios;
using std::istream;
using std::streamsize;
using std::string;
using std::unordered_map;
using std::vector;

// FIXME: Split in several methods.
vector<Level> LoaderLevel::load(
    string const& file_path, vector<Spriteset> const& spritesets,
    unordered_map<int64_t, AnimationPlayer const> const& animation_players,
    unordered_map<int64_t, TileSolidMapper const> const& tile_solid_mappers,
    vector<CharacterTemplate> const& character_templates) {
  vector<Level> levels;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      /* TODO: To be consistent with the rest, also store the index of the level
       * in the binary file? */
      int64_t width_in_tiles{StreamReader::read(stream, 2)};
      int64_t height_in_tiles{StreamReader::read(stream, 2)};
      int64_t spriteset_index{StreamReader::read(stream, 2)};
      int64_t animation_player_index{StreamReader::read(stream, 2)};
      int64_t tile_solid_mapper_index{StreamReader::read(stream, 2)};
      int64_t num_characters{StreamReader::read(stream, 2)};
      vector<Character> characters;
      for (int i{0}; i < num_characters; i++) {
        int64_t index{StreamReader::read(stream, 2)};
        int64_t x{StreamReader::read(stream, 2)};
        int64_t y{StreamReader::read(stream, 2)};
        characters.emplace_back(
            character_templates[index].spriteset(),
            character_templates[index].action_sprite_mapper(),
            character_templates[index].animation_player(),
            character_templates[index].controllers(),
            character_templates[index].solid(),
            Position{x * spritesets[spriteset_index].spritesWidth(),
                     y * spritesets[spriteset_index].spritesHeight()});
      }
      // Two bytes per tile.
      int64_t const size_in_bytes{width_in_tiles * height_in_tiles * 2};
      vector<uint16_t> tiles(size_in_bytes, 0);  // FIXME: Endianness.
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(tiles.data()),
                  static_cast<streamsize>(size_in_bytes));
      levels.emplace_back(move(tiles), width_in_tiles, height_in_tiles,
                          spritesets[spriteset_index],
                          animation_players.at(animation_player_index),
                          characters,
                          tile_solid_mappers.at(tile_solid_mapper_index));
    }
    stream.close();
  }  // FIXME: fail.

  return levels;
}
