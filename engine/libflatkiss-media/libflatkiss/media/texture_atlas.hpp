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

#ifndef LIBFLATKISS_MEDIA_TEXTURE_ATLAS_HPP_INCLUDED
#define LIBFLATKISS_MEDIA_TEXTURE_ATLAS_HPP_INCLUDED

#include <libflatkiss/media/texture.hpp>
#include <libflatkiss/model/spriteset.hpp>
#include <unordered_map>
#include <vector>

/**
 * Container for all the textures.
 */
class TextureAtlas {
 public:
  TextureAtlas(std::vector<Spriteset> const& spritesets,
               Renderer const& renderer, std::string texture_files_directory,
               std::string texture_files_prefix,
               std::string texture_files_suffix);

  Texture const& texture_for_index(int64_t texture_index) const;

 private:
  std::unordered_map<int64_t, Texture> const textures_;

  static std::unordered_map<int64_t, Texture> load(
      std::vector<Spriteset> const& spritesets, Renderer const& renderer,
      std::string texture_files_directory, std::string texture_files_prefix,
      std::string texture_files_suffix);
  static std::string texturePath(int64_t texture_index,
                                 std::string texture_files_directory,
                                 std::string texture_files_prefix,
                                 std::string texture_files_suffix);
};

#endif
