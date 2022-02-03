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
#include "Main.hpp"
#include "Navigator.hpp"
#include "PositionedRectangle.hpp"
#include "Renderer.hpp"
#include "Tileset.hpp"
#include "Vector.hpp"

size_t const CHARACTER_SIZE_PIXELS(16);
size_t const SPEED_IN_PIXELS(2);
size_t const VIEWPORT_SIZE(160);

void move(KeyboardState const& keyboard_state, Navigator const& Navigator, size_t& x, size_t& y, size_t& viewport_x, size_t& viewport_y, std::unique_ptr<Level const>& Level, Tileset const& Tileset) {
    int64_t Dx{0};
    int64_t Dy{0};
    if (keyboard_state.isPressed(SDL_SCANCODE_UP)) {
        Dy -= SPEED_IN_PIXELS;
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_DOWN)) {
        Dy += SPEED_IN_PIXELS;
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_LEFT)) {
        Dx -= SPEED_IN_PIXELS;
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_RIGHT)) {
        Dx += SPEED_IN_PIXELS;
    }

    Position NewPosition{Navigator.navigateTo(PositionedRectangle{Position{x, y}, Rectangle{CHARACTER_SIZE_PIXELS, CHARACTER_SIZE_PIXELS}}, Vector{Dx, Dy})};

    x = NewPosition.x();
    y = NewPosition.y();

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

    SDL_Window* Window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIEWPORT_SIZE, VIEWPORT_SIZE, SDL_WINDOW_SHOWN);
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
    Navigator Navigator{Collider, *Level, Tileset.tilesSize()}; // FIXME: Reference from a pointer.
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
        move(keyboard_state, Navigator, character_x, character_y, x, y, Level, Tileset);
    }

    // FIXME: Do those cleanups in error cases too.
    SDL_DestroyTexture(character);
    SDL_DestroyWindow(Window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
