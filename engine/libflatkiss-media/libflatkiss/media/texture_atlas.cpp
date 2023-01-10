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

#include <libflatkiss/media/texture_atlas.hpp>

using std::forward_as_tuple;
using std::piecewise_construct;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

TextureAtlas::TextureAtlas(std::vector<Spriteset> const& spritesets,
                           Renderer const& renderer,
                           std::string texture_files_directory,
                           std::string texture_files_prefix,
                           std::string texture_files_suffix)
    : textures_{
          TextureAtlas::load(spritesets, renderer, texture_files_directory,
                             texture_files_prefix, texture_files_suffix)} {}

unordered_map<int64_t, Texture> TextureAtlas::load(
    vector<Spriteset> const& spritesets, Renderer const& renderer,
    std::string texture_files_directory, std::string texture_files_prefix,
    std::string texture_files_suffix) {
  unordered_map<int64_t, Texture> textures_by_indices;

  for (Spriteset const& spriteset : spritesets) {
    textures_by_indices.emplace(
        piecewise_construct, forward_as_tuple(spriteset.textureIndex()),
        forward_as_tuple(
            texturePath(spriteset.textureIndex(), texture_files_directory,
                        texture_files_prefix, texture_files_suffix),
            spriteset.alpha_red(), spriteset.alpha_green(),
            spriteset.alpha_blue(), renderer));
  }

  return textures_by_indices;
}

Texture const& TextureAtlas::textureForIndex(int64_t texture_index) const {
  return textures_.at(texture_index);
}

string TextureAtlas::texturePath(int64_t texture_index,
                                 std::string texture_files_directory,
                                 std::string texture_files_prefix,
                                 std::string texture_files_suffix) {
  return texture_files_directory + "/" + texture_files_prefix +
         to_string(texture_index) + texture_files_suffix;
  // FIXME: Use std::filesystem::path as below.
  /*return (path{texture_files_directory} /
          path{texture_files_prefix + to_string(texture_index) +
               texture_files_suffix})
      .string();*/
}
