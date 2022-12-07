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

#ifndef LIBFLATKISS_MEDIA_RENDERER_HPP_INCLUDED
#define LIBFLATKISS_MEDIA_RENDERER_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <libflatkiss/media/texture.hpp>
#include <libflatkiss/model/model.hpp>
#include <unordered_map>
#include <vector>

/**
 * @brief Renders the whole scene.
 */
class Renderer {
 public:
  Renderer(SDL_Window* sdl_window);
  Renderer(Renderer const& other) = delete;
  Renderer(Renderer&& other) = delete;
  Renderer& operator=(Renderer const& other) = delete;
  Renderer& operator=(Renderer&& other) = delete;
  ~Renderer();
  /**
   * @brief Create a SDL texture that will be rendered using this renderer.
   *
   * @param surface SDL Surface to create the texture from.
   * @return SDL_Texture* Resulting texture, owned by the caller, can be
   * nullptr.
   */
  SDL_Texture* createTextureFromSurface(SDL_Surface* surface) const;
  void render(Level const& level, PositionedRectangle const& viewport,
              int64_t tick,
              std::unordered_map<int64_t, Texture> const& textures,
              std::vector<Character> const& characters) const;

 private:
  SDL_Renderer* const sdl_renderer_;

  static SDL_Rect rectForSpriteIndex(int64_t sprite_index,
                                     Spriteset const& spriteset);
  void renderCharacter(PositionedRectangle const& viewport,
                       Texture const& characterset_texture,
                       Character const& character) const;
  void renderCharacters(
      PositionedRectangle const& viewport,
      std::vector<Character> const& characters,
      std::unordered_map<int64_t, Texture> const& charactersets_textures) const;
  void renderLevel(Level const& level, Texture const& tileset_texture,
                   PositionedRectangle const& viewport, int64_t tick) const;
};

#endif
