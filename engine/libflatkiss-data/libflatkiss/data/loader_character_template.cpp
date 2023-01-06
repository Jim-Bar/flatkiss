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
#include <libflatkiss/data/loader_character_template.hpp>

using std::ifstream;
using std::ios;
using std::istream;
using std::string;
using std::unordered_map;
using std::vector;

vector<CharacterTemplate> LoaderCharacterTemplate::load(
    string const& characters_file_path, vector<Spriteset> const& spritesets,
    unordered_map<int64_t, ActionSpriteMapper const> const&
        action_sprite_mappers,
    unordered_map<int64_t, AnimationPlayer const> const& animation_players,
    unordered_map<int64_t, Solid const> const& solids) {
  vector<CharacterTemplate> character_templates;
  ifstream stream;
  stream.open(characters_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t x{0};
      int64_t y{0};
      uint16_t spriteset_index{0};
      uint16_t action_sprite_mapper_index{0};
      uint16_t animations_index{0};
      uint16_t solid_index{0};
      uint8_t controller_type{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&x), kXFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&y), kYFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&spriteset_index),
                  kSpritesetFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&action_sprite_mapper_index),
                  kIndicesFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&animations_index),
                  kAnimationFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&solid_index), kCollisionFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&controller_type),
                  kControllerFieldSize);
      character_templates.emplace_back(
          action_sprite_mappers.at(action_sprite_mapper_index),
          animation_players.at(animations_index),
          vector<ControllerType>{static_cast<ControllerType>(controller_type)},
          spritesets[spriteset_index], solids.at(solid_index));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return character_templates;
}
