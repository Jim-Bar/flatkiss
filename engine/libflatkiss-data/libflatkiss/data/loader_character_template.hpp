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

#ifndef LIBFLATKISS_DATA_LOADER_CHARACTER_TEMPLATE_HPP_INCLUDED
#define LIBFLATKISS_DATA_LOADER_CHARACTER_TEMPLATE_HPP_INCLUDED

#include <libflatkiss/model/model.hpp>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Helper class for loading the characters from a file.
 */
class LoaderCharacterTemplate {
 public:
  static std::vector<CharacterTemplate> load(
      std::string const& characters_file_path,
      std::vector<Spriteset> const& spritesets,
      std::unordered_map<int64_t, ActionSpriteMapper const> const&
          action_sprite_mappers,
      std::unordered_map<int64_t, AnimationPlayer const> const&
          animation_players,
      std::unordered_map<int64_t, Solid const> const& solids);

 private:
  static int64_t constexpr kAnimationFieldSize{2};
  static int64_t constexpr kControllerFieldSize{1};
  static int64_t constexpr kCollisionFieldSize{2};
  static int64_t constexpr kIndicesFieldSize{2};
  static int64_t constexpr kSpritesetFieldSize{2};
  static int64_t constexpr kXFieldSize{8};
  static int64_t constexpr kYFieldSize{8};
};

#endif
