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

#ifndef LIBFLATKISS_DATA_LOADER_ACTION_SPRITE_MAPPER_HPP_INCLUDED
#define LIBFLATKISS_DATA_LOADER_ACTION_SPRITE_MAPPER_HPP_INCLUDED

#include <unordered_map>
#include <string>

#include <libflatkiss/model/model.hpp>

class LoaderActionSpriteMapper {
 public:
  static std::unordered_map<int64_t, ActionSpriteMapper const> load(
      std::string const& indices_file_path);

 private:
  static Action actionIdentifierToAction(int64_t action_identifier);
  static std::unordered_map<Action, int64_t> loadGroup(int64_t group_size,
                                                       std::ifstream& stream);
};

#endif
