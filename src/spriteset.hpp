#ifndef SPRITESET_HPP_INCLUDED
#define SPRITESET_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <string>
#include <vector>

// Forward declaration to break the cycle Spriteset / Renderer.
class Renderer;

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
  Spriteset(std::string const& file_path, int64_t sprites_width,
            int64_t sprites_height, int64_t width_in_sprites,
            int64_t height_in_sprites, int64_t left_offset, int64_t top_offset,
            int64_t gap, uint8_t alpha_red, uint8_t alpha_green,
            uint8_t alpha_blue, Renderer const& renderer);
  Spriteset(Spriteset const& other) = delete;
  Spriteset(Spriteset&& other) = default;
  Spriteset& operator=(Spriteset const& other) = delete;
  Spriteset& operator=(Spriteset&& other) = delete;
  ~Spriteset();
  int64_t gap() const;
  int64_t heightInSprites() const;
  int64_t leftOffset() const;
  SDL_Rect rectForSpriteIndex(int64_t sprite_index) const;
  int64_t spritesHeight() const;
  int64_t spritesWidth() const;
  SDL_Texture* texture() const;
  int64_t topOffset() const;
  int64_t widthInSprites() const;

 private:
  int64_t const gap_;
  int64_t const height_in_sprites_;
  int64_t const left_offset_;
  int64_t const sprites_height_;
  int64_t const sprites_width_;
  SDL_Texture* const texture_;
  int64_t const top_offset_;
  int64_t const width_in_sprites_;

  /**
   * @brief For creating the texture of the spriteset in the initializer
   * list.
   *
   * @param file_path Path to the spriteset picture.
   * @param renderer Pointer to the renderer for creating the texture.
   * @param alpha_red Red component of the colour that will be turned
   * @param alpha_red Red component of the colour that will be turned
   * transparent.
   * @param alpha_green Green component of the colour that will be turned
   * transparent.
   * @param alpha_blue Blue component of the colour that will be turned
   * transparent.
   * @return SDL_Texture* Newly created texture, the caller must handle its
   * destruction.
   */
  static SDL_Texture* loadTexture(std::string const& file_path,
                                  Renderer const& renderer, uint8_t alpha_red,
                                  uint8_t alpha_green, uint8_t alpha_blue);
};

/**
 * @brief Helper class for loading the charactersets from a file.
 */
class CharactersetLoader {
 public:
  CharactersetLoader(std::string characterset_files_directory,
                     std::string characterset_files_prefix,
                     std::string characterset_files_suffix);

  std::vector<Spriteset> load(std::string const& file_path,
                              Renderer const& renderer);

 private:
  std::string const characterset_files_directory_;
  std::string const characterset_files_prefix_;
  std::string const characterset_files_suffix_;

  std::string charactersetPath(int64_t characterset) const;
};

#endif
