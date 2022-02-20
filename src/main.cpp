#include "main.hpp"

#include <SDL2/SDL.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "animation_player.hpp"
#include "collider.hpp"
#include "configuration.hpp"
#include "keyboard_state.hpp"
#include "level.hpp"
#include "navigator.hpp"
#include "positioned_rectangle.hpp"
#include "renderer.hpp"
#include "tileset.hpp"
#include "vector.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::move;
using std::unique_ptr;

size_t const kCharacterSizePixels(16);
size_t const kSpeedInPixels(2);
size_t const kViewportSize(160);

void move(KeyboardState const& keyboard_state, Navigator const& navigator,
          size_t& x, size_t& y, size_t& viewport_x, size_t& viewport_y,
          unique_ptr<Level const>& level, size_t tilesSize) {
  int64_t dx{0};
  int64_t dy{0};
  if (keyboard_state.isPressed(SDL_SCANCODE_UP)) {
    dy -= kSpeedInPixels;
  }
  if (keyboard_state.isPressed(SDL_SCANCODE_DOWN)) {
    dy += kSpeedInPixels;
  }
  if (keyboard_state.isPressed(SDL_SCANCODE_LEFT)) {
    dx -= kSpeedInPixels;
  }
  if (keyboard_state.isPressed(SDL_SCANCODE_RIGHT)) {
    dx += kSpeedInPixels;
  }

  Position new_position{navigator.moveBy(
      PositionedRectangle{Position{x, y}, Rectangle{kCharacterSizePixels,
                                                    kCharacterSizePixels}},
      Vector{dx, dy})};

  x = new_position.x();
  y = new_position.y();

  if (y < kViewportSize / 2 - kCharacterSizePixels / 2) {
    viewport_y = 0;
  } else if (y > level->heightInTiles() * tilesSize - kViewportSize / 2 -
                     kCharacterSizePixels / 2) {
    viewport_y = level->heightInTiles() * tilesSize - kViewportSize;
  } else {
    viewport_y = y - kViewportSize / 2 + kCharacterSizePixels / 2;
  }
  if (x < kViewportSize / 2 - kCharacterSizePixels / 2) {
    viewport_x = 0;
  } else if (x > level->widthInTiles() * tilesSize - kViewportSize / 2 -
                     kCharacterSizePixels / 2) {
    viewport_x = level->widthInTiles() * tilesSize - kViewportSize;
  } else {
    viewport_x = x - kViewportSize / 2 + kCharacterSizePixels / 2;
  }
}

int main(int argc, char* argv[]) {
  Configuration configuration{"configuration.ini"};
  unique_ptr<Level const> level{move(LevelLoader::load(
      configuration.levelPath(), configuration.levelWidthInTiles(),
      configuration.levelHeightInTiles()))};

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    cerr << "SDL_Init Error: " << SDL_GetError() << endl;
    return EXIT_FAILURE;
  }

  SDL_Window* window = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, kViewportSize,
                                        kViewportSize, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
    return EXIT_FAILURE;
  }

  Renderer renderer{window};

  Tileset const tileset{
      configuration.tilesetPath(),         configuration.tilesetTilesSize(),
      configuration.tilesetWidthInTiles(), configuration.tilesetHeightInTiles(),
      configuration.tilesetLeftOffset(),   configuration.tilesetTopOffset(),
      configuration.tilesetGap(),          renderer};

  SDL_Surface* character_surface = SDL_LoadBMP("assets/character.bmp");
  if (character_surface == nullptr) {
    cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Texture* character = renderer.createTextureFromSurface(character_surface);
  if (character == nullptr) {
    cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }
  SDL_FreeSurface(character_surface);

  bool quit = false;
  SDL_Event event;
  KeyboardState keyboard_state;
  AnimationPlayer animation_player{
      AnimationLoader::load(configuration.animationsPath())};
  Collider collider{CollisionLoader::load(configuration.collisionsPath())};
  Navigator navigator{collider, *level, tileset.tilesSize()};
  size_t x{0};
  size_t y{0};
  size_t character_x{0};
  size_t character_y{0};
  size_t tick(0);
  while (!quit) {
    renderer.render(animation_player, *level, tileset, x, y, kViewportSize,
                    tick++, character, character_x, character_y,
                    kCharacterSizePixels);
    SDL_Delay(configuration.engineTickDurationMs());
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        cout << "Program quit after " << event.quit.timestamp << " ticks"
             << endl;
        quit = true;
      } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        keyboard_state.update(event.key.keysym.scancode,
                              event.key.state == SDL_PRESSED);
      }
    }
    move(keyboard_state, navigator, character_x, character_y, x, y, level,
         tileset.tilesSize());
  }

  SDL_DestroyTexture(character);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
