#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <unordered_map>
#include <vector>

#include "character.hpp"
#include "level.hpp"
#include "positioned_rectangle.hpp"
#include "texture.hpp"

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
