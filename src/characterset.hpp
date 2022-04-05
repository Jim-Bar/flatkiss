#ifndef CHARACTERSET_HPP_INCLUDED
#define CHARACTERSET_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <array>
#include <string>
#include <vector>

#include "configuration.hpp"
#include "move_direction.hpp"

// Forward declaration to break the cycle Characterset / Renderer.
class Renderer;

// FIXME: Merge with Spriteset?
/**
 * @brief Models a characterset.
 *
 * A characterset is a picture containing multiple sprites. It provides handy
 * methods to get information on the characterset and to easily render sprites.
 *
 * This class also handles the lifecycle of the texture of the characterset.
 */
class Characterset {
 public:
  Characterset(std::string const& file_path, int64_t sprites_width,
               int64_t sprites_height, int64_t width_in_sprites,
               int64_t height_in_sprites, int64_t left_offset,
               int64_t top_offset, int64_t gap, uint8_t alpha_red,
               uint8_t alpha_green, uint8_t alpha_blue,
               int64_t sprite_move_left_index, int64_t sprite_move_down_index,
               int64_t sprite_move_right_index, int64_t sprite_move_up_index,
               Renderer const& renderer);
  Characterset(Characterset const& other) = default;
  Characterset(Characterset&& other) = delete;
  Characterset& operator=(Characterset const& other) = delete;
  Characterset& operator=(Characterset&& other) = delete;
  ~Characterset();
  int64_t gap() const;
  int64_t heightInSprites() const;
  int64_t leftOffset() const;
  SDL_Rect rectForMoveDirection(MoveDirection const& move_direction) const;
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
  std::array<int64_t, 4> sprites_move_directions_indices_;
  SDL_Texture* const texture_;
  int64_t const top_offset_;
  int64_t const width_in_sprites_;

  /**
   * @brief For creating the texture of the characterset in the initializer
   * list.
   *
   * @param file_path Path to the characterset picture.
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
  static std::vector<Characterset> load(std::string const& file_path,
                                        Configuration const& configuration,
                                        Renderer const& renderer);
};

#endif
