#include "texture.hpp"

#include <iostream>  // FIXME: Remove.

#include "renderer.hpp"

using std::forward_as_tuple;
using std::piecewise_construct;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

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

    return texture;
  }

  // TODO: Raise exception.
  std::cerr << "Failed to load texture " << file_path << std::endl;

  return nullptr;
}

SDL_Texture* Texture::texture() const { return texture_; }

TextureLoader::TextureLoader(string texture_files_directory,
                             string texture_files_prefix,
                             string texture_files_suffix)
    : texture_files_directory_{texture_files_directory},
      texture_files_prefix_{texture_files_prefix},
      texture_files_suffix_{texture_files_suffix} {}

string TextureLoader::texturePath(int64_t texture_index) const {
  return texture_files_directory_ + "/" + texture_files_prefix_ +
         to_string(texture_index) + texture_files_suffix_;
  // FIXME: Use std::filesystem::path as below.
  /*return (path{texture_files_directory_} /
          path{texture_files_prefix_ + to_string(texture_index) +
               texture_files_suffix_})
      .string();*/
}

unordered_map<int64_t, Texture> TextureLoader::load(
    vector<Spriteset> const& spritesets, Renderer const& renderer) {
  unordered_map<int64_t, Texture> textures_by_indices;

  for (Spriteset const& spriteset : spritesets) {
    textures_by_indices.emplace(
        piecewise_construct, forward_as_tuple(spriteset.textureIndex()),
        forward_as_tuple(texturePath(spriteset.textureIndex()),
                         spriteset.alpha_red(), spriteset.alpha_green(),
                         spriteset.alpha_blue(), renderer));
  }

  return textures_by_indices;
}
