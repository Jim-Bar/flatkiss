#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <SDL2/SDL.h>

#include "animation_player.hpp"
#include "character.hpp"
#include "level.hpp"
#include "positioned_rectangle.hpp"
#include "tileset.hpp"

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
  void render(AnimationPlayer const& animation_player, Level const& level,
              Tileset const& tileset, PositionedRectangle const& viewport,
              size_t tick, SDL_Texture* character_texture,
              Character const& character) const;

 private:
  SDL_Renderer* const sdl_renderer_;

  void renderCharacter(PositionedRectangle const& viewport, size_t tick,
                       SDL_Texture* character_texture,
                       Character const& character) const;
  void renderLevel(AnimationPlayer const& animation_player, Level const& level,
                   Tileset const& tileset, PositionedRectangle const& viewport,
                   size_t tick) const;
};

#endif
