#ifndef TILESET_HPP_INCLUDED
#define TILESET_HPP_INCLUDED

#include <SDL2/SDL.h>
#include <string>

/**
 * @brief Models a tileset.
 *
 * A tileset is a picture containing multiple tiles. It provides handy methods to get information on the tileset and to
 * easily render tiles.
 *
 * This class also handles the lifecycle of the texture of the tileset. 
 */
class Tileset {
public:
    Tileset(std::string const& FilePath, size_t TilesSize, size_t WidthInTiles, size_t HeightInTiles, size_t LeftOffset, size_t TopOffset, size_t Gap, SDL_Renderer* Renderer);
    ~Tileset();
    size_t const gap() const;
    size_t const heightInTiles() const;
    size_t const leftOffset() const;
    SDL_Rect rectForTileIndex(size_t tile_index) const;
    SDL_Texture* texture() const;
    size_t const tilesSize() const;
    size_t const topOffset() const;
    size_t const widthInTiles() const;

private:
    size_t const Gap;
    size_t const HeightInTiles;
    size_t const LeftOffset;
    SDL_Texture* const Texture;
    size_t const TilesSize;
    size_t const TopOffset;
    size_t const WidthInTiles;

    /**
     * @brief For creating the texture of the tileset in the initializer list.
     * 
     * @param FilePath Path to the tileset picture.
     * @param Renderer Pointer to the renderer for creating the texture, not owned by this class.
     * @return Newly created texture, the caller must handle its destruction.
     */
    static SDL_Texture* loadTexture(std::string const& FilePath, SDL_Renderer* Renderer);
};

#endif
