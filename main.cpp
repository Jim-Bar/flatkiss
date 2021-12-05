#include <cstdlib>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>
#include <string>

size_t const TILE_SIZE_PIXELS(16);
size_t const TILESET_WIDTH_TILES(5);
size_t const LEVEL_WIDTH_TILES(10);
size_t const LEVEL_HEIGHT_TILES(LEVEL_WIDTH_TILES);
size_t const LEVEL_SIZE_TILES(LEVEL_WIDTH_TILES * LEVEL_HEIGHT_TILES);
size_t const VIEWPORT_SIZE(10 * TILE_SIZE_PIXELS);

void load_level(uint8_t level[], size_t level_size, std::string level_file_name) {
    std::ifstream stream;
    stream.open(level_file_name, std::ios::in | std::ios::binary);
    if (stream.is_open()) {
        stream.read(reinterpret_cast<char*>(level), level_size);
        stream.close();
    }
}

void render_level(uint8_t level[], size_t level_width, size_t level_height, SDL_Renderer *renderer, SDL_Texture *tileset,
                  size_t tileset_width_tiles, size_t tile_size_pixels) {
    for (size_t y(0); y < level_height; y++) {
        for (size_t x(0); x < level_width; x++) {
            uint8_t tile_index(level[y * level_width + x]);

            SDL_Rect source_rect, dest_rect;
            source_rect.w = tile_size_pixels;
            source_rect.h = source_rect.w;
            dest_rect.w = source_rect.w;
            dest_rect.h = source_rect.h;

            // + 1 is for the line separatings tiles in the tileset.
            source_rect.x = (tile_index % tileset_width_tiles) * (source_rect.w + 1);
            source_rect.y = (tile_index / tileset_width_tiles) * (source_rect.h + 1);

            dest_rect.x = x * source_rect.w;
            dest_rect.y = y * source_rect.h;

            SDL_RenderCopy(renderer, tileset, &source_rect, &dest_rect);
        }
    }
}

int main(int argc, char *argv[])
{
    using std::cerr;
    using std::endl;

    uint8_t level[LEVEL_SIZE_TILES] = {0};
    load_level(level, LEVEL_SIZE_TILES, "levels/level.bin");

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
		if (win != nullptr) {
			SDL_DestroyWindow(win);
		}
		SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Surface* bmp = SDL_LoadBMP("assets/tileset.bmp");
    if (bmp == nullptr) {
        cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
		if (ren != nullptr) {
			SDL_DestroyRenderer(ren);
		}
		if (win != nullptr) {
			SDL_DestroyWindow(win);
		}
		SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture* tileset = SDL_CreateTextureFromSurface(ren, bmp);
    if (tileset == nullptr) {
        cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
		if (bmp != nullptr) {
			SDL_FreeSurface(bmp);
		}
		if (ren != nullptr) {
			SDL_DestroyRenderer(ren);
		}
		if (win != nullptr) {
			SDL_DestroyWindow(win);
		}
		SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_FreeSurface(bmp);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        SDL_RenderClear(ren);
        render_level(level, LEVEL_WIDTH_TILES, LEVEL_HEIGHT_TILES, ren, tileset, TILESET_WIDTH_TILES, TILE_SIZE_PIXELS);
        SDL_RenderPresent(ren);
        SDL_Delay(100);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Log("Program quit after %i ticks", event.quit.timestamp);
                quit = true;
            }
        }
    }

    SDL_DestroyTexture(tileset);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
