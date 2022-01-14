#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

#include "AnimationPlayer.hpp"
#include "Collider.hpp"
#include "Configuration.hpp"
#include "KeyboardState.hpp"
#include "Level.hpp"
#include "Renderer.hpp"
#include "Tileset.hpp"

size_t const CHARACTER_SIZE_PIXELS(16);
size_t const SPEED_IN_PIXELS(2);
size_t const VIEWPORT_SIZE(160);

void move(KeyboardState const& keyboard_state, Collider const& Collider, size_t& x, size_t& y, size_t& viewport_x, size_t& viewport_y, std::unique_ptr<Level const>& Level, Tileset const& Tileset) {
    size_t NewX{x};
    size_t NewY{y};
    if (keyboard_state.isPressed(SDL_SCANCODE_UP)) {
        if (y < SPEED_IN_PIXELS) {
            NewY = 0;
        } else {
            NewY -= SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_DOWN)) {
        if (y + CHARACTER_SIZE_PIXELS + SPEED_IN_PIXELS >= Level->heightInTiles() * Tileset.tilesSize()) {
            NewY = Level->heightInTiles() * Tileset.tilesSize() - CHARACTER_SIZE_PIXELS;
        } else {
            NewY += SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_LEFT)) {
        if (x < SPEED_IN_PIXELS) {
            NewX = 0;
        } else {
            NewX -= SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_RIGHT)) {
        if (x + CHARACTER_SIZE_PIXELS + SPEED_IN_PIXELS >= Level->widthInTiles() * Tileset.tilesSize()) {
            NewX = Level->widthInTiles() * Tileset.tilesSize() - CHARACTER_SIZE_PIXELS;
        } else {
            NewX += SPEED_IN_PIXELS;
        }
    }

    if (x == NewX && y == NewY) {
        return;
    }

    for (size_t Y(NewY / Tileset.tilesSize()); Y <= (NewY + CHARACTER_SIZE_PIXELS) / Tileset.tilesSize(); Y++) {
        for (size_t X(NewX / Tileset.tilesSize()); X <= (NewX + CHARACTER_SIZE_PIXELS) / Tileset.tilesSize(); X++) {
            uint16_t TileIndex(Level->tileIndex(X, Y));
            if (Collider.collides(TileIndex, NewX, NewY, X * Tileset.tilesSize(), Y * Tileset.tilesSize(), CHARACTER_SIZE_PIXELS)) {
                return;
            }
        }
    }

    x = NewX;
    y = NewY;

    if (y < VIEWPORT_SIZE / 2 - CHARACTER_SIZE_PIXELS / 2) {
        viewport_y = 0;
    } else if (y > Level->heightInTiles() * Tileset.tilesSize() - VIEWPORT_SIZE / 2 - CHARACTER_SIZE_PIXELS / 2) {
        viewport_y = Level->heightInTiles() * Tileset.tilesSize() - VIEWPORT_SIZE;
    } else {
        viewport_y = y - VIEWPORT_SIZE / 2 + CHARACTER_SIZE_PIXELS / 2;
    }
    if (x < VIEWPORT_SIZE / 2 - CHARACTER_SIZE_PIXELS / 2) {
        viewport_x = 0;
    } else if (x > Level->widthInTiles() * Tileset.tilesSize() - VIEWPORT_SIZE / 2 - CHARACTER_SIZE_PIXELS / 2) {
        viewport_x = Level->widthInTiles() * Tileset.tilesSize() - VIEWPORT_SIZE;
    } else {
        viewport_x = x - VIEWPORT_SIZE / 2 + CHARACTER_SIZE_PIXELS / 2;
    }
}

int main(int argc, char *argv[])
{
    using std::cerr;
    using std::endl;

    Configuration Configuration{"configuration.ini"};
    std::unique_ptr<Level const> Level{LevelLoader::load(Configuration.levelPath(), Configuration.levelWidthInTiles(), Configuration.levelHeightInTiles())};

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_Window* Window = SDL_CreateWindow("2d-rendering-engine-test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIEWPORT_SIZE, VIEWPORT_SIZE, SDL_WINDOW_SHOWN);
    if (Window == nullptr) {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    Renderer Renderer{Window};

    Tileset const Tileset{Configuration.tilesetPath(), Configuration.tilesetTilesSize(), Configuration.tilesetWidthInTiles(),
        Configuration.tilesetHeightInTiles(), Configuration.tilesetLeftOffset(), Configuration.tilesetTopOffset(), Configuration.tilesetGap(), Renderer};

    SDL_Surface* characterSurface = SDL_LoadBMP("assets/character.bmp");
    if (characterSurface == nullptr) {
        cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture* character = Renderer.createTextureFromSurface(characterSurface);
    if (character == nullptr) {
        cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_FreeSurface(characterSurface);

    bool quit = false;
    SDL_Event event;
    KeyboardState keyboard_state;
    AnimationPlayer AnimationPlayer{AnimationLoader::load(Configuration.animationsPath())};
    Collider Collider{CollisionLoader::load(Configuration.collisionsPath())};
    size_t x(0), y(0), character_x(0), character_y(0);
    size_t Tick(0);
    while (!quit) {
        Renderer.render(AnimationPlayer, Level, Tileset, x, y, VIEWPORT_SIZE, Tick++, character, character_x, character_y, CHARACTER_SIZE_PIXELS);
        SDL_Delay(Configuration.engineTickDurationMs());
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Log("Program quit after %i ticks", event.quit.timestamp);
                quit = true;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                keyboard_state.update(event.key.keysym.scancode, event.key.state == SDL_PRESSED ? true : false);
            }
        }
        move(keyboard_state, Collider, character_x, character_y, x, y, Level, Tileset);
    }

    // FIXME: Do those cleanups in error cases too.
    SDL_DestroyTexture(character);
    SDL_DestroyWindow(Window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
