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
  Tileset(std::string const& file_path, size_t tiles_size,
          size_t width_in_tiles, size_t height_in_tiles, size_t left_offset,
          size_t top_offset, size_t gap, Renderer const& renderer);
  ~Tileset();
  size_t gap() const;
  size_t heightInTiles() const;
  size_t leftOffset() const;
  SDL_Rect rectForTileIndex(size_t tile_index) const;
  SDL_Texture* texture() const;
  size_t tilesSize() const;
  size_t topOffset() const;
  size_t widthInTiles() const;

 private:
  size_t const gap_;
  size_t const height_in_tiles_;
  size_t const left_offset_;
  SDL_Texture* const texture_;
  /* FIXME: Probably that does not belong to the tileset (but to the model
   * instead).*/
  size_t const tiles_size_;
  size_t const top_offset_;
  size_t const width_in_tiles_;

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
