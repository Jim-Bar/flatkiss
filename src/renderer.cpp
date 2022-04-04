#include "renderer.hpp"

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
                      Level const& level, Tileset const& tileset,
                      PositionedRectangle const& viewport, int64_t tick,
                      vector<Characterset> const& charactersets,
                      vector<Character> const& characters) const {
  SDL_RenderClear(sdl_renderer_);
  renderLevel(animation_player, level, tileset, viewport, tick);
  for (Character const& character : characters) {
    renderCharacter(viewport, tick, character.characterset(), character);
  }
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::renderCharacter(PositionedRectangle const& viewport,
                               int64_t tick, Characterset const& characterset,
                               Character const& character) const {
  SDL_Rect source_rect{
      characterset.rectForMoveDirection(character.movingDirection())};
  SDL_Rect dest_rect;
  dest_rect.x = static_cast<int>(character.x() - viewport.x());
  dest_rect.y = static_cast<int>(character.y() - viewport.y());
  dest_rect.w = static_cast<int>(character.width());
  dest_rect.h = static_cast<int>(character.height());

  SDL_RenderCopy(sdl_renderer_, characterset.texture(), &source_rect,
                 &dest_rect);
}

void Renderer::renderLevel(AnimationPlayer const& animation_player,
                           Level const& level, Tileset const& tileset,
                           PositionedRectangle const& viewport,
                           int64_t tick) const {
  for (int64_t y(viewport.y() / tileset.tilesSize());
       y <= (viewport.y() + viewport.height()) / tileset.tilesSize(); y++) {
    for (int64_t x(viewport.x() / tileset.tilesSize());
         x <= (viewport.x() + viewport.width()) / tileset.tilesSize(); x++) {
      uint16_t tile_index(
          animation_player.animatedTileIndexFor(level.tileIndex(x, y), tick));

      SDL_Rect source_rect{tileset.rectForTileIndex(tile_index)};
      SDL_Rect dest_rect;
      dest_rect.w = static_cast<int>(tileset.tilesSize());
      dest_rect.h = static_cast<int>(tileset.tilesSize());
      dest_rect.x = static_cast<int>(x * tileset.tilesSize() - viewport.x());
      dest_rect.y = static_cast<int>(y * tileset.tilesSize() - viewport.y());

      SDL_RenderCopy(sdl_renderer_, tileset.texture(), &source_rect,
                     &dest_rect);
    }
  }
}
