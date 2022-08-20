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

#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "spriteset.hpp"

// Forward declaration to break the cycle Spriteset / Renderer.
class Renderer;

class Texture {
 public:
  Texture(std::string const& file_path, uint8_t alpha_red, uint8_t alpha_green,
          uint8_t alpha_blue, Renderer const& renderer);
  Texture(Texture const& other) = delete;
  Texture(Texture&& other) = default;
  Texture& operator=(Texture const& other) = delete;
  Texture& operator=(Texture&& other) = delete;
  ~Texture();
  SDL_Texture* texture() const;

 private:
  SDL_Texture* const texture_;

  /**
   * @brief For creating the texture of the spriteset in the initializer
   * list.
   *
   * @param file_path Path to the picture to load in the texture.
   * @param renderer Pointer to the renderer for creating the texture.
   * @param alpha_red Red component of the colour that will be turned
   * @param alpha_red Red component of the colour that will be turned
   * transparent.
   * @param alpha_green Green component of the colour that will be turned
   * transparent.
   * @param alpha_blue Blue component of the colour that will be turned
   * transparent.
   * @return SDL_Texture* Newly created texture, the caller must handle its
   * destruction.
   */
  static SDL_Texture* loadTexture(std::string const& file_path,
                                  Renderer const& renderer, uint8_t alpha_red,
                                  uint8_t alpha_green, uint8_t alpha_blue);
};

/**
 * @brief Helper class for loading the textures from a file.
 */
class TextureLoader {
 public:
  TextureLoader(std::string texture_files_directory,
                std::string texture_files_prefix,
                std::string texture_files_suffix);

  std::unordered_map<int64_t, Texture> load(
      std::vector<Spriteset> const& spritesets, Renderer const& renderer);

 private:
  std::string const texture_files_directory_;
  std::string const texture_files_prefix_;
  std::string const texture_files_suffix_;

  std::string texturePath(int64_t texture_index) const;
};

#endif
