#include "texture.hpp"

#include "renderer.hpp"
#include "spriteset.hpp"

using std::string;

Texture::Texture(string const& file_path, uint8_t alpha_red,
                 uint8_t alpha_green, uint8_t alpha_blue,
                 Renderer const& renderer)
    : texture_{Texture::loadTexture(file_path, renderer, alpha_red, alpha_green,
                                    alpha_blue)} {}

Texture::~Texture() { SDL_DestroyTexture(texture_); }

SDL_Texture* Texture::loadTexture(string const& file_path,
                                  Renderer const& renderer, uint8_t alpha_red,
                                  uint8_t alpha_green, uint8_t alpha_blue) {
  SDL_Surface* surface = SDL_LoadBMP(file_path.c_str());
  SDL_Texture* texture = nullptr;
  if (surface != nullptr) {
    SDL_SetColorKey(
        surface, SDL_TRUE,
        SDL_MapRGB(surface->format, alpha_red, alpha_green, alpha_blue));
    texture = renderer.createTextureFromSurface(
        surface);  // TODO: Check nullity / raise exception.
    SDL_FreeSurface(surface);
  }  // TODO: Raise exception.

  return texture;
}

SDL_Texture* Texture::texture() const { return texture_; }
