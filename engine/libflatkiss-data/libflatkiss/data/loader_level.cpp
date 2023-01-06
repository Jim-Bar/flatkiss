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

// FIXME: Remove this, the best thing to do is probably to put the characters
// after the tiles in the binary file for the level, and then reading the
// characters after the level.
struct CharacterTemp {
 public:
  CharacterTemp(CharacterTemplate const& character_template,
                Position const position)
      : character_template_{character_template}, position_{position} {};
  CharacterTemplate const& character_template_;
  Position const position_;
};

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
      uint16_t width_in_tiles{0};
      uint16_t height_in_tiles{0};
      uint16_t spriteset_index{0};
      uint16_t animation_player_index{0};
      uint16_t tile_solid_mapper_index{0};
      uint16_t num_characters{0};
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
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&num_characters), 2);
      vector<CharacterTemp> characters_temp;
      for (int i{0}; i < num_characters; i++) {
        uint16_t index{0};
        uint16_t x{0};
        uint16_t y{0};
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        stream.read(reinterpret_cast<char*>(&index), 2);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        stream.read(reinterpret_cast<char*>(&x), 2);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        stream.read(reinterpret_cast<char*>(&y), 2);
        characters_temp.emplace_back(
            character_templates[index],
            Position{x * spritesets[spriteset_index].spritesWidth(),
                     y * spritesets[spriteset_index].spritesHeight()});
      }
      // Two bytes per tile.
      int64_t const size_in_bytes{width_in_tiles * height_in_tiles * 2};
      auto tiles{vector<uint16_t>(size_in_bytes, 0)};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(tiles.data()),
                  static_cast<streamsize>(size_in_bytes));
      vector<Character> characters;
      for (CharacterTemp temp : characters_temp) {
        characters.emplace_back(temp.character_template_.spriteset(),
                                temp.character_template_.action_sprite_mapper(),
                                temp.character_template_.animation_player(),
                                temp.character_template_.controllers(),
                                temp.character_template_.solid(),
                                temp.position_);
      }
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
