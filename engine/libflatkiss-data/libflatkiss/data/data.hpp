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

#ifndef LIBFLATKISS_DATA_DATA_HPP_INCLUDED
#define LIBFLATKISS_DATA_DATA_HPP_INCLUDED

#include <libflatkiss/model/model.hpp>
#include <string>
#include <vector>

class Data {
 public:
  Data(std::string const& action_sprite_maps_path,
             std::string const& animations_path,
             std::string const& characters_path, std::string const& levels_path,
             std::string const& solids_path, std::string const& spritesets_path,
             std::string const& tile_solid_maps_path);
  Model load() const;

 private:
  std::string const action_sprite_maps_path_;
  std::string const animations_path_;
  std::string const characters_path_;
  std::string const levels_path_;
  std::string const solids_path_;
  std::string const spritesets_path_;
  std::string const tile_solid_maps_path_;
};

#endif
