#include "tileset.hpp"

#include "renderer.hpp"

using std::string;

Tileset::Tileset(string const& file_path, int64_t tiles_size,
                 int64_t width_in_tiles, int64_t height_in_tiles,
                 int64_t left_offset, int64_t top_offset, int64_t gap,
                 Renderer const& renderer)
    : tiles_size_{tiles_size},
      width_in_tiles_{width_in_tiles},
      height_in_tiles_{height_in_tiles},
      left_offset_{left_offset},
      top_offset_{top_offset},
      gap_{gap},
      texture_{Tileset::loadTexture(file_path, renderer)} {}

Tileset::~Tileset() { SDL_DestroyTexture(texture_); }

int64_t Tileset::gap() const { return gap_; }

int64_t Tileset::heightInTiles() const { return height_in_tiles_; }

int64_t Tileset::leftOffset() const { return left_offset_; }

SDL_Texture* Tileset::loadTexture(string const& file_path,
                                  Renderer const& renderer) {
  SDL_Surface* surface = SDL_LoadBMP(file_path.c_str());
  SDL_Texture* texture = nullptr;
  if (surface != nullptr) {
    texture = renderer.createTextureFromSurface(
        surface);  // TODO: Check nullity / raise exception.
    SDL_FreeSurface(surface);
  }  // TODO: Raise exception.

  return texture;
}

SDL_Rect Tileset::rectForTileIndex(int64_t tile_index) const {
  SDL_Rect source_rect;
  source_rect.w = static_cast<int>(tilesSize());
  source_rect.h = static_cast<int>(tilesSize());

  source_rect.x = static_cast<int>(
      (tile_index % widthInTiles()) * (source_rect.w + gap()) + leftOffset());
  source_rect.y = static_cast<int>(
      (tile_index / widthInTiles()) * (source_rect.h + gap()) + topOffset());

  return source_rect;
}

SDL_Texture* Tileset::texture() const { return texture_; }

int64_t Tileset::tilesSize() const { return tiles_size_; }

int64_t Tileset::topOffset() const { return top_offset_; }

int64_t Tileset::widthInTiles() const { return width_in_tiles_; }
