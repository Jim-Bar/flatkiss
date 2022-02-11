#include "renderer.hpp"

Renderer::Renderer(SDL_Window* sdl_window)
    : sdl_renderer_(SDL_CreateRenderer(
          sdl_window, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
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
                      size_t viewport_x, size_t viewport_y,
                      size_t viewport_size_in_pixels, size_t tick,
                      SDL_Texture* character_texture, size_t character_x,
                      size_t character_y,
                      size_t character_size_in_pixels) const {
  SDL_RenderClear(sdl_renderer_);
  renderLevel(animation_player, level, tileset, viewport_x, viewport_y,
              viewport_size_in_pixels, tick);
  renderCharacter(viewport_x, viewport_y, tick, character_texture, character_x,
                  character_y, character_size_in_pixels);
  SDL_RenderPresent(sdl_renderer_);
}

void Renderer::renderCharacter(size_t viewport_x, size_t viewport_y,
                               size_t tick, SDL_Texture* character_texture,
                               size_t character_x, size_t character_y,
                               size_t character_size_in_pixels) const {
  SDL_Rect dest_rect;
  dest_rect.x = static_cast<int>(character_x - viewport_x);
  dest_rect.y = static_cast<int>(character_y - viewport_y);
  dest_rect.w = static_cast<int>(character_size_in_pixels);
  dest_rect.h = static_cast<int>(character_size_in_pixels);

  SDL_RenderCopy(sdl_renderer_, character_texture, nullptr, &dest_rect);
}

void Renderer::renderLevel(AnimationPlayer const& animation_player,
                           Level const& level, Tileset const& tileset,
                           size_t viewport_x, size_t viewport_y,
                           size_t viewport_size_in_pixels, size_t tick) const {
  for (size_t y(viewport_y / tileset.tilesSize());
       y <= (viewport_y + viewport_size_in_pixels) / tileset.tilesSize(); y++) {
    for (size_t x(viewport_x / tileset.tilesSize());
         x <= (viewport_x + viewport_size_in_pixels) / tileset.tilesSize();
         x++) {
      uint16_t tile_index(
          animation_player.animatedTileIndexFor(level.tileIndex(x, y), tick));

      SDL_Rect source_rectangle{tileset.rectForTileIndex(tile_index)};
      SDL_Rect destination_rectangle;
      destination_rectangle.w = static_cast<int>(tileset.tilesSize());
      destination_rectangle.h = static_cast<int>(tileset.tilesSize());
      destination_rectangle.x =
          static_cast<int>(x * tileset.tilesSize() - viewport_x);
      destination_rectangle.y =
          static_cast<int>(y * tileset.tilesSize() - viewport_y);

      SDL_RenderCopy(sdl_renderer_, tileset.texture(), &source_rectangle,
                     &destination_rectangle);
    }
  }
}
