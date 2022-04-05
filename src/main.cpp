#include "main.hpp"

#include <SDL2/SDL.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "animation_player.hpp"
#include "character.hpp"
#include "characterset.hpp"
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
using std::vector;

int64_t const kCharacterSizePixels(16);
int64_t const kSpeedInPixels(1);
int64_t const kViewportSize(160);

void handleKeyboardEvent(KeyboardState const& keyboard_state,
                         Character& character, PositionedRectangle& viewport,
                         unique_ptr<Level const>& level, int64_t tilesSize) {
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

  character.moveBy(Vector{dx, dy});

  if (character.y() <
      viewport.rectangle().height() / 2 - character.height() / 2) {
    viewport.y(0);
  } else if (character.y() > level->heightInTiles() * tilesSize -
                                 viewport.rectangle().height() / 2 -
                                 character.height() / 2) {
    viewport.y(level->heightInTiles() * tilesSize -
               viewport.rectangle().height());
  } else {
    viewport.y(character.y() - viewport.rectangle().height() / 2 +
               character.height() / 2);
  }
  if (character.x() <
      viewport.rectangle().width() / 2 - character.width() / 2) {
    viewport.x(0);
  } else if (character.x() > level->widthInTiles() * tilesSize -
                                 viewport.rectangle().width() / 2 -
                                 character.width() / 2) {
    viewport.x(level->widthInTiles() * tilesSize -
               viewport.rectangle().width());
  } else {
    viewport.x(character.x() - viewport.rectangle().width() / 2 +
               character.width() / 2);
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

  PositionedRectangle viewport{Position{0, 0},
                               Rectangle{kViewportSize, kViewportSize}};
  SDL_Window* window = SDL_CreateWindow(
      PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      static_cast<int>(viewport.rectangle().width()),
      static_cast<int>(viewport.rectangle().height()), SDL_WINDOW_SHOWN);
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

  vector<Characterset> charactersets{CharactersetLoader::load(
      configuration.charactersetsPath(),
      configuration /* FIXME: Should not need to pass configuration */,
      renderer)};

  bool quit = false;
  SDL_Event event;
  KeyboardState keyboard_state;
  AnimationPlayer animation_player{
      AnimationLoader::load(configuration.animationsPath())};
  Collider collider{CollisionLoader::load(configuration.collisionsPath())};
  Navigator navigator{collider, *level, tileset.tilesSize()};
  vector<Character> characters{
      CharacterLoader::load(configuration.charactersPath(), charactersets,
                            navigator, tileset.tilesSize())};
  int64_t tick(0);
  while (!quit) {
    renderer.render(animation_player, *level, tileset, viewport, tick++,
                    charactersets, characters);
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
    // FIXME: Not all characters are keyboard-controlled.
    for (Character& character : characters) {
      handleKeyboardEvent(keyboard_state, character, viewport, level,
                          tileset.tilesSize());
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
