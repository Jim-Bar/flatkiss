#include "Renderer.hpp"

Renderer::Renderer(SDL_Window* SDLWindow) : SDLRenderer(SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
    if (SDLRenderer == nullptr) {
        // FIXME: Raise an exception.
    }
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(SDLRenderer);
}

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* Surface) const {
    return SDL_CreateTextureFromSurface(SDLRenderer, Surface);
}

void Renderer::render(AnimationPlayer const& AnimationPlayer, Level const& Level, Tileset const& Tileset,
                size_t ViewPortX, size_t ViewPortY, size_t ViewPortSizeInPixels, size_t Tick, SDL_Texture* CharacterTexture,
                size_t CharacterX, size_t CharacterY, size_t CharacterSizeInPixels) const {
    SDL_RenderClear(SDLRenderer);
    renderLevel(AnimationPlayer, Level, Tileset, ViewPortX, ViewPortY, ViewPortSizeInPixels, Tick);
    renderCharacter(ViewPortX, ViewPortY, Tick, CharacterTexture, CharacterX, CharacterY, CharacterSizeInPixels);
    SDL_RenderPresent(SDLRenderer);
}

void Renderer::renderCharacter(size_t ViewPortX, size_t ViewPortY, size_t Tick, SDL_Texture* CharacterTexture, size_t CharacterX, size_t CharacterY, size_t CharacterSizeInPixels) const {
    SDL_Rect DestRect;
    DestRect.x = CharacterX - ViewPortX;
    DestRect.y = CharacterY - ViewPortY;
    DestRect.w = CharacterSizeInPixels;
    DestRect.h = CharacterSizeInPixels;

    SDL_RenderCopy(SDLRenderer, CharacterTexture, nullptr, &DestRect);
}

void Renderer::renderLevel(AnimationPlayer const& AnimationPlayer, Level const& Level,
                           Tileset const& Tileset, size_t ViewPortX, size_t ViewPortY, size_t ViewPortSizeInPixels, size_t Tick) const {
    for (size_t Y(ViewPortY / Tileset.tilesSize()); Y <= (ViewPortY + ViewPortSizeInPixels) / Tileset.tilesSize(); Y++) {
        for (size_t X(ViewPortX / Tileset.tilesSize()); X <= (ViewPortX + ViewPortSizeInPixels) / Tileset.tilesSize(); X++) {
            uint16_t TileIndex(AnimationPlayer.animatedTileIndexFor(Level.tileIndex(X, Y), Tick));

            SDL_Rect SourceRect{Tileset.rectForTileIndex(TileIndex)};
            SDL_Rect DestRect;
            DestRect.w = Tileset.tilesSize();
            DestRect.h = Tileset.tilesSize();
            DestRect.x = X * Tileset.tilesSize() - ViewPortX;
            DestRect.y = Y * Tileset.tilesSize() - ViewPortY;

            SDL_RenderCopy(SDLRenderer, Tileset.texture(), &SourceRect, &DestRect);
        }
    }
}
