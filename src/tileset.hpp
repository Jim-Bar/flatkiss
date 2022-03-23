#ifndef TILESET_HPP_INCLUDED
#define TILESET_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <string>

// Forward declaration to break the cycle Tileset / Renderer.
class Renderer;

/**
 * @brief Models a tileset.
 *
 * A tileset is a picture containing multiple tiles. It provides handy methods
 * to get information on the tileset and to easily render tiles.
 *
 * This class also handles the lifecycle of the texture of the tileset.
 */
class Tileset {
 public:
  Tileset(std::string const& file_path, int64_t tiles_size,
          int64_t width_in_tiles, int64_t height_in_tiles, int64_t left_offset,
          int64_t top_offset, int64_t gap, Renderer const& renderer);
  Tileset(Tileset const& other) = delete;
  Tileset(Tileset&& other) = delete;
  Tileset& operator=(Tileset const& other) = delete;
  Tileset& operator=(Tileset&& other) = delete;
  ~Tileset();
  int64_t gap() const;
  int64_t heightInTiles() const;
  int64_t leftOffset() const;
  SDL_Rect rectForTileIndex(int64_t tile_index) const;
  SDL_Texture* texture() const;
  int64_t tilesSize() const;
  int64_t topOffset() const;
  int64_t widthInTiles() const;

 private:
  int64_t const gap_;
  int64_t const height_in_tiles_;
  int64_t const left_offset_;
  SDL_Texture* const texture_;
  int64_t const tiles_size_;
  int64_t const top_offset_;
  int64_t const width_in_tiles_;

  /**
   * @brief For creating the texture of the tileset in the initializer list.
   *
   * @param file_path Path to the tileset picture.
   * @param renderer Pointer to the renderer for creating the texture.
   * @return SDL_Texture* Newly created texture, the caller must handle its
   * destruction.
   */
  static SDL_Texture* loadTexture(std::string const& file_path,
                                  Renderer const& renderer);
};

#endif
