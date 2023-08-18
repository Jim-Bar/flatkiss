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

#include <algorithm>
#include <libflatkiss/media/renderer.hpp>
#include <stdexcept>

using std::runtime_error;
using std::sort;
using std::unordered_map;
using std::vector;

Renderer::Renderer(SDL_Window* sdl_window)
    : sdl_renderer_{SDL_CreateRenderer(
          sdl_window, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)} {
  if (sdl_renderer_ == nullptr) {
    throw runtime_error("Failed to create SDL renderer");
  }
}

Renderer::~Renderer() { SDL_DestroyRenderer(sdl_renderer_); }

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* surface) const {
  return SDL_CreateTextureFromSurface(sdl_renderer_, surface);
}

SDL_Rect Renderer::rectForSpriteIndex(int64_t sprite_index,
                                      Spriteset const& spriteset) {
  SDL_Rect source_rect;
  source_rect.w = static_cast<int>(spriteset.spritesWidth());
  source_rect.h = static_cast<int>(spriteset.spritesHeight());

  source_rect.x = static_cast<int>((sprite_index % spriteset.widthInSprites()) *
                                       (source_rect.w + spriteset.gap()) +
                                   spriteset.leftOffset());
  source_rect.y = static_cast<int>((sprite_index / spriteset.widthInSprites()) *
                                       (source_rect.h + spriteset.gap()) +
                                   spriteset.topOffset());

  return source_rect;
}

void Renderer::render(Level const& level, PositionedRectangle const& viewport,
                      int64_t tick, TextureAtlas const& textures,
                      vector<Character> const& characters) const {
  SDL_RenderClear(sdl_renderer_);
  renderLevel(level, textures.textureForIndex(level.spriteset().textureIndex()),
              viewport, tick);
  renderCharacters(viewport, characters, textures);
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::renderCharacter(PositionedRectangle const& viewport,
                               Texture const& characterset_texture,
                               Character const& character) const {
  SDL_Rect source_rect{
      rectForSpriteIndex(character.spriteIndex(), character.spriteset())};
  SDL_Rect dest_rect;
  dest_rect.x = static_cast<int>(character.x() - viewport.x());
  dest_rect.y = static_cast<int>(character.y() - viewport.y());
  dest_rect.w = static_cast<int>(character.spriteset().spritesWidth());
  dest_rect.h = static_cast<int>(character.spriteset().spritesHeight());

  SDL_RenderCopy(sdl_renderer_, characterset_texture.texture(), &source_rect,
                 &dest_rect);
}

void Renderer::renderCharacters(
    PositionedRectangle const& viewport, vector<Character> const& characters,
    TextureAtlas const& charactersets_textures) const {
  /* The characters with the lower positions on the Y-axis must appear behind
   * the others. Sort them using their Y-positions. However it is better not to
   * modify the input vector (plus it is const). A copy could be made, but then
   * sorting it would require Character to be copy-assignable (i.e. no const
   * attributes), so that is not possible either. Instead create a vector of
   * indices to the character, and sort that instead. Easy! See:
   * https://stackoverflow.com/a/47537314 */

  // Vector of indices.
  vector<int64_t> character_indices;
  for (int64_t i{0}; i < characters.size(); i++) {
    character_indices.push_back(i);
  }

  // Sorted by Y-position.
  sort(character_indices.begin(), character_indices.end(),
       [characters](int64_t left, int64_t right) {
         return characters[left].position().y() <
                characters[right].position().y();
       });

  // Render the characters from top-most to bottom-most.
  for (int64_t character_index : character_indices) {
    Character const& character = characters[character_index];
    renderCharacter(viewport,
                    charactersets_textures.textureForIndex(
                        character.spriteset().textureIndex()),
                    character);
  }
}

void Renderer::renderLevel(Level const& level, Texture const& tileset_texture,
                           PositionedRectangle const& viewport,
                           int64_t tick) const {
  Spriteset const& tileset = level.spriteset();
  for (int64_t y(viewport.y() / tileset.spritesHeight());
       y <= (viewport.y() + viewport.height()) / tileset.spritesHeight(); y++) {
    for (int64_t x(viewport.x() / tileset.spritesWidth());
         x <= (viewport.x() + viewport.width()) / tileset.spritesWidth(); x++) {
      uint16_t tile_index(level.animationPlayer().animatedSpriteIndexFor(
          level.tileIndex(x, y), tick));

      SDL_Rect source_rect{rectForSpriteIndex(tile_index, tileset)};
      SDL_Rect dest_rect;
      dest_rect.w = static_cast<int>(tileset.spritesWidth());
      dest_rect.h = static_cast<int>(tileset.spritesHeight());
      dest_rect.x = static_cast<int>(x * tileset.spritesWidth() - viewport.x());
      dest_rect.y =
          static_cast<int>(y * tileset.spritesHeight() - viewport.y());

      SDL_RenderCopy(sdl_renderer_, tileset_texture.texture(), &source_rect,
                     &dest_rect);
    }
  }
}
