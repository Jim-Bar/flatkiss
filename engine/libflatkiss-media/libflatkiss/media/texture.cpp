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

#include <SDL2/SDL.h>

#include <libflatkiss/media/renderer.hpp>
#include <libflatkiss/media/texture.hpp>
#include <stdexcept>

using std::runtime_error;
using std::string;
using std::unordered_map;
using std::vector;

Texture::Texture(string const& file_path, uint8_t alpha_red,
                 uint8_t alpha_green, uint8_t alpha_blue,
                 Renderer const& renderer)
    : texture_{Texture::loadTexture(file_path, renderer, alpha_red, alpha_green,
                                    alpha_blue)} {}

Texture::~Texture() { SDL_DestroyTexture(texture_); }

SDL_Texture* Texture::loadTexture(string const& file_path,
                                  Renderer const& renderer, uint8_t alpha_red,
                                  uint8_t alpha_green, uint8_t alpha_blue) {
  SDL_Surface* surface = SDL_LoadBMP(file_path.c_str());
  SDL_Texture* texture = nullptr;
  if (surface != nullptr) {
    SDL_SetColorKey(
        surface, SDL_TRUE,
        SDL_MapRGB(surface->format, alpha_red, alpha_green, alpha_blue));
    texture = renderer.createTextureFromSurface(surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
      throw runtime_error("Failed to create SDL texture");
    }

    return texture;
  }

  throw runtime_error("Failed to load SDL surface: " + file_path);
}

SDL_Texture* Texture::texture() const { return texture_; }
