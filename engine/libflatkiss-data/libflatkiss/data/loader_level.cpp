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
#include <libflatkiss/model/level.hpp>

using std::ifstream;
using std::ios;
using std::istream;
using std::streamsize;
using std::string;
using std::unordered_map;
using std::vector;

/* TODO: To be consistent with the rest, also store the index of the level
 * in the binary file? */
struct DataLevelInfo {
  uint16_t width_in_tiles;
  uint16_t height_in_tiles;
  uint16_t spriteset_index;
  uint16_t animation_player_index;
  uint16_t tile_solid_mapper_index;
  uint16_t num_characters;
};

struct DataCharacterTemplate {
  uint16_t index;
  uint16_t x;
  uint16_t y;
};

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
      DataLevelInfo level_info{};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&level_info), sizeof(DataLevelInfo));
      vector<Character> characters;
      for (int i{0}; i < level_info.num_characters; i++) {
        DataCharacterTemplate character{};
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        stream.read(reinterpret_cast<char*>(&character),
                    sizeof(DataCharacterTemplate));
        characters.emplace_back(
            character_templates[character.index].spriteset(),
            character_templates[character.index].action_sprite_mapper(),
            character_templates[character.index].animation_player(),
            character_templates[character.index].controllers(),
            character_templates[character.index].solid(),
            Position{
                character.x *
                    spritesets[level_info.spriteset_index].spritesWidth(),
                character.y *
                    spritesets[level_info.spriteset_index].spritesHeight()});
      }
      // Two bytes per tile.
      int64_t const size_in_bytes{level_info.width_in_tiles *
                                  level_info.height_in_tiles * 2};
      auto tiles{vector<uint16_t>(size_in_bytes, 0)};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(tiles.data()),
                  static_cast<streamsize>(size_in_bytes));
      levels.emplace_back(
          move(tiles), level_info.width_in_tiles, level_info.height_in_tiles,
          spritesets[level_info.spriteset_index],
          animation_players.at(level_info.animation_player_index), characters,
          tile_solid_mappers.at(level_info.tile_solid_mapper_index));
    }
    stream.close();
  }  // FIXME: fail.

  return levels;
}
