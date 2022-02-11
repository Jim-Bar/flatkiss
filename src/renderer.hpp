#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <SDL2/SDL.h>

#include "animation_player.hpp"
#include "level.hpp"
#include "tileset.hpp"

/**
 * @brief Renders the whole scene.
 */
class Renderer {
 public:
  Renderer(SDL_Window* sdl_window);
  ~Renderer();
  /**
   * @brief Create a SDL texture that will be rendered using this renderer.
   *
   * @param surface SDL Surface to create the texture from.
   * @return SDL_Texture* Resulting texture, owned by the caller, can be
   * nullptr.
   */
  SDL_Texture* createTextureFromSurface(SDL_Surface* surface) const;
  void render(AnimationPlayer const& animation_player, Level const& level,
              Tileset const& tileset, size_t viewport_x, size_t viewport_y,
              size_t viewport_size_in_pixels, size_t tick,
              SDL_Texture* character_texture, size_t character_x,
              size_t character_y, size_t character_size_in_pixels) const;

 private:
  SDL_Renderer* const sdl_renderer_;

  void renderCharacter(size_t viewport_x, size_t viewport_y, size_t tick,
                       SDL_Texture* character_texture, size_t character_x,
                       size_t character_y,
                       size_t character_size_in_pixels) const;
  void renderLevel(AnimationPlayer const& animation_player, Level const& level,
                   Tileset const& tileset, size_t viewport_x, size_t viewport_y,
                   size_t viewport_size_in_pixels, size_t tick) const;
};

#endif
