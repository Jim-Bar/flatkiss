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

#ifndef LIBFLATKISS_DATA_LOADER_LEVEL_HPP_INCLUDED
#define LIBFLATKISS_DATA_LOADER_LEVEL_HPP_INCLUDED

#include <libflatkiss/model/model.hpp>
#include <unordered_map>
#include <vector>

/**
 * @brief Helper class for loading the level from a file.
 */
class LoaderLevel {
 public:
  static std::vector<Level> load(
      std::string const& file_path, std::vector<Spriteset> const& spritesets,
      std::unordered_map<int64_t, AnimationPlayer const> const&
          animation_players,
      std::unordered_map<int64_t, TileSolidMapper const> const&
          tile_solid_mappers,
      std::vector<CharacterTemplate> const& character_templates);
};

#endif
