#ifndef SPRITESET_HPP_INCLUDED
#define SPRITESET_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <string>
#include <tuple>
#include <vector>

#include "texture.hpp"

/**
 * @brief Models a spriteset.
 *
 * A spriteset is a picture containing multiple sprites. It provides handy
 * methods to get information on the spriteset and to easily render sprites.
 * Charactersets and tilesets are in fact spritesets.
 *
 * This class also handles the lifecycle of the texture of the spriteset.
 */
class Spriteset {
 public:
  Spriteset(int64_t sprites_width, int64_t sprites_height,
            int64_t width_in_sprites, int64_t height_in_sprites,
            int64_t left_offset, int64_t top_offset, int64_t gap);
  Spriteset(Spriteset const& other) = delete;
  Spriteset(Spriteset&& other) = default;
  Spriteset& operator=(Spriteset const& other) = delete;
  Spriteset& operator=(Spriteset&& other) = delete;
  int64_t gap() const;
  int64_t heightInSprites() const;
  int64_t leftOffset() const;
  SDL_Rect rectForSpriteIndex(int64_t sprite_index) const;
  int64_t spritesHeight() const;
  int64_t spritesWidth() const;
  int64_t topOffset() const;
  int64_t widthInSprites() const;

 private:
  int64_t const gap_;
  int64_t const height_in_sprites_;
  int64_t const left_offset_;
  int64_t const sprites_height_;
  int64_t const sprites_width_;
  int64_t const top_offset_;
  int64_t const width_in_sprites_;
};

/**
 * @brief Helper class for loading the charactersets from a file.
 */
class CharactersetLoader {
 public:
  CharactersetLoader(std::string texture_files_directory,
                     std::string texture_files_prefix,
                     std::string texture_files_suffix);

  std::tuple<std::vector<Spriteset>, std::vector<Texture>> load(
      std::string const& file_path, Renderer const& renderer);

 private:
  std::string const texture_files_directory_;
  std::string const texture_files_prefix_;
  std::string const texture_files_suffix_;

  std::string texturePath(int64_t texture_index) const;
};

#endif
