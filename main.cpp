#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

size_t const TILE_SIZE_PIXELS(16);
size_t const TILESET_WIDTH_TILES(24);
size_t const LEVEL_WIDTH_TILES(20);
size_t const LEVEL_HEIGHT_TILES(LEVEL_WIDTH_TILES);
size_t const LEVEL_SIZE_TILES(LEVEL_WIDTH_TILES * LEVEL_HEIGHT_TILES);
size_t const SPEED_IN_PIXELS(3);
size_t const VIEWPORT_SIZE(10 * TILE_SIZE_PIXELS);

class KeyboardState {
public:
    bool is_pressed(SDL_Scancode key) const {
        switch (key) {
            case SDL_SCANCODE_UP:
                return up;
            case SDL_SCANCODE_DOWN:
                return down;
            case SDL_SCANCODE_LEFT:
                return left;
            case SDL_SCANCODE_RIGHT:
                return right;
        }

        return false; // FIXME: Exception.
    }

    void update(SDL_Scancode key, bool pressed) {
        switch (key) {
            case SDL_SCANCODE_UP:
                up = pressed;
                return;
            case SDL_SCANCODE_DOWN:
                down = pressed;
                return;
            case SDL_SCANCODE_LEFT:
                left = pressed;
                return;
            case SDL_SCANCODE_RIGHT:
                right = pressed;
                return;
        }
    }

private:
    bool up{false}, down{false}, left{false}, right{false};
};

class Animation {
public:
  Animation(std::unique_ptr<uint16_t const[]> tile_indices, uint8_t period, uint8_t duration)
    : tile_indices(std::move(tile_indices)), period(period), duration(duration) {}
  uint8_t get_period() const {
      return period;
  }
  uint8_t get_duration() const {
      return duration;
  }
  uint16_t tile_index_at_step(uint16_t step) const {
      return tile_indices[step];
  }

private:
  uint8_t const duration;
  std::unique_ptr<uint16_t const[]> tile_indices;
  uint8_t const period;
};

class Animations {
public:
    Animations() {
        std::ifstream stream;
        stream.open("assets/animations.bin", std::ios::in | std::ios::binary);
        if (stream.is_open()) {
            char byte{0};
            while ((byte = stream.get()) != std::istream::traits_type::eof()) {
                uint8_t animation_period{static_cast<uint8_t>(byte)};
                uint8_t animation_duration{static_cast<uint8_t>(stream.get())};
                auto animation = std::make_unique<uint16_t[]>(animation_period);
                stream.read(reinterpret_cast<char*>(animation.get()), animation_period * 2); // Two byte per tile.
                animations_per_tile_index.emplace(std::piecewise_construct, std::forward_as_tuple(animation[0]), std::forward_as_tuple(std::move(animation), animation_period, animation_duration));
            }
            stream.close();
        }
    }

  uint16_t animated_tile_index_for(uint16_t tile_index, size_t tick) const {
        if (animations_per_tile_index.find(tile_index) == animations_per_tile_index.end()) { // FIXME: Use contains()
            return tile_index;
        }

        Animation const& animation{animations_per_tile_index.at(tile_index)};
        return animation.tile_index_at_step(((tick % (animation.get_period() * animation.get_duration())) / animation.get_duration()));
    }

private:
    std::unordered_map<uint16_t, Animation> animations_per_tile_index;
};

void load_level(uint16_t level[], size_t level_size, std::string level_file_name) {
    std::ifstream stream;
    stream.open(level_file_name, std::ios::in | std::ios::binary);
    if (stream.is_open()) {
        stream.read(reinterpret_cast<char*>(level), level_size * 2); // Two byte per tile.
        stream.close();
    }
}

void render_level(Animations const& animations, size_t tick, uint16_t level[], size_t level_width, size_t level_height, SDL_Renderer *renderer, SDL_Texture *tileset,
                  size_t tileset_width_tiles, size_t tile_size_pixels, size_t current_x, size_t current_y) {
    for (size_t y(current_y / tile_size_pixels); y <= (current_y + VIEWPORT_SIZE) / tile_size_pixels; y++) {
        for (size_t x(current_x / tile_size_pixels); x <= (current_x + VIEWPORT_SIZE) / tile_size_pixels; x++) {
            uint16_t tile_index(animations.animated_tile_index_for(level[y * level_width + x], tick));

            SDL_Rect source_rect, dest_rect;
            source_rect.w = tile_size_pixels;
            source_rect.h = source_rect.w;
            dest_rect.w = source_rect.w;
            dest_rect.h = source_rect.h;

            // + 1 is for the line separatings tiles in the tileset.
            source_rect.x = (tile_index % tileset_width_tiles) * (source_rect.w + 1);
            source_rect.y = (tile_index / tileset_width_tiles) * (source_rect.h + 1);

            dest_rect.x = x * source_rect.w - current_x;
            dest_rect.y = y * source_rect.h - current_y;

            SDL_RenderCopy(renderer, tileset, &source_rect, &dest_rect);
        }
    }
}

void move(KeyboardState const& keyboard_state, size_t& x, size_t& y) {
    if (keyboard_state.is_pressed(SDL_SCANCODE_UP)) {
        if (y < SPEED_IN_PIXELS) {
            y = 0;
        } else {
            y -= SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.is_pressed(SDL_SCANCODE_DOWN)) {
        if (y + VIEWPORT_SIZE >= LEVEL_HEIGHT_TILES * TILE_SIZE_PIXELS) {
            y = LEVEL_HEIGHT_TILES * TILE_SIZE_PIXELS - VIEWPORT_SIZE;
        } else {
            y += SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.is_pressed(SDL_SCANCODE_LEFT)) {
        if (x < SPEED_IN_PIXELS) {
            x = 0;
        } else {
            x -= SPEED_IN_PIXELS;
        }
    }
    if (keyboard_state.is_pressed(SDL_SCANCODE_RIGHT)) {
        if (x + VIEWPORT_SIZE >= LEVEL_WIDTH_TILES * TILE_SIZE_PIXELS) {
            x = LEVEL_WIDTH_TILES * TILE_SIZE_PIXELS - VIEWPORT_SIZE;
        } else {
            x += SPEED_IN_PIXELS;
        }
    }
}

int main(int argc, char *argv[])
{
    using std::cerr;
    using std::endl;

    uint16_t level[LEVEL_SIZE_TILES] = {0};
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
    KeyboardState keyboard_state;
    Animations animations;
    size_t x(0), y(0);
    size_t tick(0);
    while (!quit) {
        SDL_RenderClear(ren);
        render_level(animations, tick++, level, LEVEL_WIDTH_TILES, LEVEL_HEIGHT_TILES, ren, tileset, TILESET_WIDTH_TILES, TILE_SIZE_PIXELS, x, y);
        SDL_RenderPresent(ren);
        SDL_Delay(16);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Log("Program quit after %i ticks", event.quit.timestamp);
                quit = true;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                keyboard_state.update(event.key.keysym.scancode, event.key.state == SDL_PRESSED ? true : false);
            }
        }
        move(keyboard_state, x, y);
    }

    SDL_DestroyTexture(tileset);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
