#include "renderer.hpp"

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
                      PositionedRectangle const& viewport, size_t tick,
                      SDL_Texture* character_texture,
                      Character const& character) const {
  SDL_RenderClear(sdl_renderer_);
  renderLevel(animation_player, level, tileset, viewport, tick);
  renderCharacter(viewport, tick, character_texture, character);
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::renderCharacter(PositionedRectangle const& viewport, size_t tick,
                               SDL_Texture* character_texture,
                               Character const& character) const {
  SDL_Rect dest_rect;
  dest_rect.x = static_cast<int>(character.x() - viewport.x());
  dest_rect.y = static_cast<int>(character.y() - viewport.y());
  dest_rect.w = static_cast<int>(character.width());
  dest_rect.h = static_cast<int>(character.height());

  SDL_RenderCopy(sdl_renderer_, character_texture, nullptr, &dest_rect);
}

void Renderer::renderLevel(AnimationPlayer const& animation_player,
                           Level const& level, Tileset const& tileset,
                           PositionedRectangle const& viewport,
                           size_t tick) const {
  for (size_t y(viewport.y() / tileset.tilesSize());
       y <= (viewport.y() + viewport.height()) / tileset.tilesSize(); y++) {
    for (size_t x(viewport.x() / tileset.tilesSize());
         x <= (viewport.x() + viewport.width()) / tileset.tilesSize(); x++) {
      uint16_t tile_index(
          animation_player.animatedTileIndexFor(level.tileIndex(x, y), tick));

      SDL_Rect source_rectangle{tileset.rectForTileIndex(tile_index)};
      SDL_Rect destination_rectangle;
      destination_rectangle.w = static_cast<int>(tileset.tilesSize());
      destination_rectangle.h = static_cast<int>(tileset.tilesSize());
      destination_rectangle.x =
          static_cast<int>(x * tileset.tilesSize() - viewport.x());
      destination_rectangle.y =
          static_cast<int>(y * tileset.tilesSize() - viewport.y());

      SDL_RenderCopy(sdl_renderer_, tileset.texture(), &source_rectangle,
                     &destination_rectangle);
    }
  }
}
