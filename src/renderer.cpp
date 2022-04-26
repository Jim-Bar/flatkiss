#include "renderer.hpp"

#include <algorithm>

using std::sort;
using std::vector;

Renderer::Renderer(SDL_Window* sdl_window)
    : sdl_renderer_{SDL_CreateRenderer(
          sdl_window, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)} {
  if (sdl_renderer_ == nullptr) {
    // FIXME: Raise an exception.
  }
}

Renderer::~Renderer() { SDL_DestroyRenderer(sdl_renderer_); }

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* surface) const {
  return SDL_CreateTextureFromSurface(sdl_renderer_, surface);
}

void Renderer::render(AnimationPlayer const& animation_player,
                      Level const& level, Spriteset const& tileset,
                      PositionedRectangle const& viewport, int64_t tick,
                      vector<Spriteset> const& charactersets,
                      vector<Character> const& characters) const {
  SDL_RenderClear(sdl_renderer_);
  renderLevel(animation_player, level, tileset, viewport, tick);
  renderCharacters(viewport, tick, characters);
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::renderCharacter(PositionedRectangle const& viewport,
                               int64_t tick, Spriteset const& characterset,
                               Character const& character) const {
  SDL_Rect source_rect{
      characterset.rectForSpriteIndex(character.spriteIndex())};
  SDL_Rect dest_rect;
  dest_rect.x = static_cast<int>(character.x() - viewport.x());
  dest_rect.y = static_cast<int>(character.y() - viewport.y());
  dest_rect.w = static_cast<int>(character.characterset().spritesWidth());
  dest_rect.h = static_cast<int>(character.characterset().spritesHeight());

  SDL_RenderCopy(sdl_renderer_, characterset.texture(), &source_rect,
                 &dest_rect);
}

void Renderer::renderCharacters(PositionedRectangle const& viewport,
                                int64_t tick,
                                vector<Character> const& characters) const {
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
    renderCharacter(viewport, tick, characters[character_index].characterset(),
                    characters[character_index]);
  }
}

void Renderer::renderLevel(AnimationPlayer const& animation_player,
                           Level const& level, Spriteset const& tileset,
                           PositionedRectangle const& viewport,
                           int64_t tick) const {
  for (int64_t y(viewport.y() / tileset.spritesHeight());
       y <= (viewport.y() + viewport.height()) / tileset.spritesHeight(); y++) {
    for (int64_t x(viewport.x() / tileset.spritesWidth());
         x <= (viewport.x() + viewport.width()) / tileset.spritesWidth(); x++) {
      uint16_t tile_index(
          animation_player.animatedSpriteIndexFor(level.tileIndex(x, y), tick));

      SDL_Rect source_rect{tileset.rectForSpriteIndex(tile_index)};
      SDL_Rect dest_rect;
      dest_rect.w = static_cast<int>(tileset.spritesWidth());
      dest_rect.h = static_cast<int>(tileset.spritesHeight());
      dest_rect.x = static_cast<int>(x * tileset.spritesWidth() - viewport.x());
      dest_rect.y =
          static_cast<int>(y * tileset.spritesHeight() - viewport.y());

      SDL_RenderCopy(sdl_renderer_, tileset.texture(), &source_rect,
                     &dest_rect);
    }
  }
}
