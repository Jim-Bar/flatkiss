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

#ifndef CONFIGURATION_HPP_INCLUDED
#define CONFIGURATION_HPP_INCLUDED

#include <string>

/**
 * @brief Provider of configurable constants.
 *
 * Loads a configuration file in the INI style, and provides getters on the
 * constants from that file.
 */
class Configuration {
 public:
  Configuration(std::string const& file_path);
  std::string const& actionSpriteMapsPath() const;
  std::string animationsPath() const;
  std::string const& charactersPath() const;
  int64_t engineTickDurationMs() const;
  std::string const& levelsPath() const;
  std::string const& solidsPath() const;
  std::string spritesetFilesDirectory() const;
  std::string spritesetFilesPrefix() const;
  std::string spritesetFilesSuffix() const;
  std::string const& spritesetsPath() const;
  std::string const& tileSolidMapsPath() const;

 private:
  std::string action_sprite_maps_path_{};
  std::string animations_path_{};
  std::string characters_path_{};
  int64_t engine_tick_duration_ms_{0};
  std::string levels_path_{};
  std::string solids_path_{};
  std::string spriteset_files_directory_{};
  std::string spriteset_files_prefix_{};
  std::string spriteset_files_suffix_{};
  std::string spritesets_path_{};
  std::string tile_solid_maps_path_{};
};

#endif
