#include "Renderer.hpp"
#include "Tileset.hpp"

using std::string;

Tileset::Tileset(string const& FilePath, size_t TilesSize, size_t WidthInTiles, size_t HeightInTiles, size_t LeftOffset, size_t TopOffset, size_t Gap, Renderer const& Renderer) :
    TilesSize(TilesSize), WidthInTiles(WidthInTiles), HeightInTiles(HeightInTiles), LeftOffset(LeftOffset), TopOffset(TopOffset), Gap(Gap), Texture(Tileset::loadTexture(FilePath, Renderer)) {

}

Tileset::~Tileset() {
    SDL_DestroyTexture(Texture);
}

size_t const Tileset::gap() const {
    return Gap;
}

size_t const Tileset::heightInTiles() const {
    return HeightInTiles;
}

size_t const Tileset::leftOffset() const {
    return LeftOffset;
}

SDL_Texture* Tileset::loadTexture(std::string const& FilePath, Renderer const& Renderer) {
    SDL_Surface* Surface = SDL_LoadBMP(FilePath.c_str());
    SDL_Texture* Texture = nullptr;
    if (Surface != nullptr) {
        Texture = Renderer.createTextureFromSurface(Surface); // TODO: Check nullity / raise exception.
        SDL_FreeSurface(Surface);
    } // TODO: Raise exception.

    return Texture;
}

SDL_Rect Tileset::rectForTileIndex(size_t tile_index) const {
    SDL_Rect source_rect;
    source_rect.w = tilesSize();
    source_rect.h = tilesSize();

    source_rect.x = (tile_index % widthInTiles()) * (source_rect.w + gap()) + leftOffset();
    source_rect.y = (tile_index / widthInTiles()) * (source_rect.h + gap()) + topOffset();

    return source_rect;
}

SDL_Texture* Tileset::texture() const {
    return Texture;
}

size_t const Tileset::tilesSize() const {
    return TilesSize;
}

size_t const Tileset::topOffset() const {
    return TopOffset;
}

size_t const Tileset::widthInTiles() const {
    return WidthInTiles;
}
