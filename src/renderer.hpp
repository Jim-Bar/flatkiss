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
  Renderer(SDL_Window* SDLWindow);
  ~Renderer();
  /**
   * @brief Create a SDL texture that will be rendered using this renderer.
   *
   * @param Surface SDL Surface to create the texture from.
   * @return SDL_Texture* Resulting texture, owned by the caller, can be
   * nullptr.
   */
  SDL_Texture* createTextureFromSurface(SDL_Surface* Surface) const;
  void render(AnimationPlayer const& AnimationPlayer, Level const& Level,
              Tileset const& Tileset, size_t ViewPortX, size_t ViewPortY,
              size_t ViewPortSizeInPixels, size_t Tick,
              SDL_Texture* CharacterTexture, size_t CharacterX,
              size_t CharacterY, size_t CharacterSizeInPixels) const;

 private:
  SDL_Renderer* const SDLRenderer;

  void renderCharacter(size_t ViewPortX, size_t ViewPortY, size_t Tick,
                       SDL_Texture* CharacterTexture, size_t CharacterX,
                       size_t CharacterY, size_t CharacterSizeInPixels) const;
  void renderLevel(AnimationPlayer const& AnimationPlayer, Level const& Level,
                   Tileset const& Tileset, size_t ViewPortX, size_t ViewPortY,
                   size_t ViewPortSizeInPixels, size_t Tick) const;
};

#endif
