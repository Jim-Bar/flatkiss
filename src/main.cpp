#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

#include "AnimationPlayer.hpp"
#include "Configuration.hpp"
#include "KeyboardState.hpp"
#include "Level.hpp"
#include "Tileset.hpp"

size_t const CHARACTER_SIZE_PIXELS(16);
size_t const SPEED_IN_PIXELS(2);
size_t const VIEWPORT_SIZE(160);

void render_level(AnimationPlayer const& AnimationPlayer, size_t tick, std::unique_ptr<Level const>& Level, SDL_Renderer *renderer,
                  Tileset const& Tileset, size_t current_x, size_t current_y) {
    for (size_t y(current_y / Tileset.tilesSize()); y <= (current_y + VIEWPORT_SIZE) / Tileset.tilesSize(); y++) {
        for (size_t x(current_x / Tileset.tilesSize()); x <= (current_x + VIEWPORT_SIZE) / Tileset.tilesSize(); x++) {
            uint16_t tile_index(AnimationPlayer.animatedTileIndexFor(Level->tileIndex(x, y), tick));

            SDL_Rect source_rect{Tileset.rectForTileIndex(tile_index)};
            SDL_Rect dest_rect;
            dest_rect.w = Tileset.tilesSize();
            dest_rect.h = Tileset.tilesSize();
            dest_rect.x = x * Tileset.tilesSize() - current_x;
            dest_rect.y = y * Tileset.tilesSize() - current_y;

            SDL_RenderCopy(renderer, Tileset.texture(), &source_rect, &dest_rect);
        }
    }
}

void render_character(SDL_Renderer *renderer, SDL_Texture *character, size_t character_x, size_t character_y, size_t viewport_x, size_t viewport_y) {
    SDL_Rect dest_rect;
    dest_rect.x = character_x - viewport_x;
    dest_rect.y = character_y - viewport_y;
    dest_rect.w = CHARACTER_SIZE_PIXELS;
    dest_rect.h = CHARACTER_SIZE_PIXELS;

    SDL_RenderCopy(renderer, character, nullptr, &dest_rect);
}

void move(KeyboardState const& keyboard_state, size_t& x, size_t& y, size_t& viewport_x, size_t& viewport_y, std::unique_ptr<Level const>& Level, Tileset const& Tileset) {
    if (keyboard_state.isPressed(SDL_SCANCODE_UP)) {
        if (y < SPEED_IN_PIXELS) {
            y = 0;
        } else {
            y -= SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_DOWN)) {
        if (y + CHARACTER_SIZE_PIXELS + SPEED_IN_PIXELS >= Level->heightInTiles() * Tileset.tilesSize()) {
            y = Level->heightInTiles() * Tileset.tilesSize() - CHARACTER_SIZE_PIXELS;
        } else {
            y += SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_LEFT)) {
        if (x < SPEED_IN_PIXELS) {
            x = 0;
        } else {
            x -= SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.isPressed(SDL_SCANCODE_RIGHT)) {
        if (x + CHARACTER_SIZE_PIXELS + SPEED_IN_PIXELS >= Level->widthInTiles() * Tileset.tilesSize()) {
            x = Level->widthInTiles() * Tileset.tilesSize() - CHARACTER_SIZE_PIXELS;
        } else {
            x += SPEED_IN_PIXELS;
        }
    }

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

    SDL_Window* win = SDL_CreateWindow("Hello World!", 100, 100, VIEWPORT_SIZE, VIEWPORT_SIZE, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_Renderer* ren
        = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        cerr << "SDL_CreateRenderer Error" << SDL_GetError() << endl;
		SDL_Quit();
        return EXIT_FAILURE;
    }

    Tileset const Tileset{Configuration.tilesetPath(), Configuration.tilesetTilesSize(), Configuration.tilesetWidthInTiles(),
        Configuration.tilesetHeightInTiles(), Configuration.tilesetLeftOffset(), Configuration.tilesetTopOffset(), Configuration.tilesetGap(), ren};

    SDL_Surface* characterSurface = SDL_LoadBMP("assets/character.bmp");
    if (characterSurface == nullptr) {
        cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture* character = SDL_CreateTextureFromSurface(ren, characterSurface);
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
    size_t x(0), y(0), character_x(0), character_y(0);
    size_t tick(0);
    while (!quit) {
        SDL_RenderClear(ren);
        render_level(AnimationPlayer, tick++, Level, ren, Tileset, x, y);
        render_character(ren, character, character_x, character_y, x, y);
        SDL_RenderPresent(ren);
        SDL_Delay(Configuration.engineTickDurationMs());
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Log("Program quit after %i ticks", event.quit.timestamp);
                quit = true;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                keyboard_state.update(event.key.keysym.scancode, event.key.state == SDL_PRESSED ? true : false);
            }
        }
        move(keyboard_state, character_x, character_y, x, y, Level, Tileset);
    }

    // FIXME: Do those cleanups in error cases too.
    SDL_DestroyTexture(character);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
