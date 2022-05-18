#include "main.hpp"

#include <SDL2/SDL.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "action_sprite_mapper.hpp"
#include "animation_player.hpp"
#include "character.hpp"
#include "character_controller.hpp"
#include "configuration.hpp"
#include "keyboard_state.hpp"
#include "level.hpp"
#include "navigator.hpp"
#include "positioned_rectangle.hpp"
#include "renderer.hpp"
#include "spriteset.hpp"
#include "tile_solid_mapper.hpp"
#include "vector.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::move;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

int64_t const kCharacterSizePixels(16);
int64_t const kViewportSize(160);

void updateViewport(Character const& character, PositionedRectangle& viewport,
                    Level const& level, int64_t tiles_width,
                    int64_t tiles_height) {
  if (character.y() < viewport.rectangle().height() / 2 -
                          character.characterset().spritesHeight() / 2) {
    viewport.y(0);
  } else if (character.y() > level.heightInTiles() * tiles_height -
                                 viewport.rectangle().height() / 2 -
                                 character.characterset().spritesHeight() / 2) {
    viewport.y(level.heightInTiles() * tiles_height -
               viewport.rectangle().height());
  } else {
    viewport.y(character.y() - viewport.rectangle().height() / 2 +
               character.characterset().spritesHeight() / 2);
  }
  if (character.x() < viewport.rectangle().width() / 2 -
                          character.characterset().spritesWidth() / 2) {
    viewport.x(0);
  } else if (character.x() > level.widthInTiles() * tiles_width -
                                 viewport.rectangle().width() / 2 -
                                 character.characterset().spritesWidth() / 2) {
    viewport.x(level.widthInTiles() * tiles_width -
               viewport.rectangle().width());
  } else {
    viewport.x(character.x() - viewport.rectangle().width() / 2 +
               character.characterset().spritesWidth() / 2);
  }
}

int main(int argc, char* argv[]) {
  Configuration configuration{"configuration.ini"};
  Level const level{move(LevelLoader::load(
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

  Spriteset const tileset{configuration.tilesetPath(),
                          configuration.tilesetTilesSize(),
                          configuration.tilesetTilesSize(),
                          configuration.tilesetWidthInTiles(),
                          configuration.tilesetHeightInTiles(),
                          configuration.tilesetLeftOffset(),
                          configuration.tilesetTopOffset(),
                          configuration.tilesetGap(),
                          255,
                          0,
                          255,
                          renderer};

  CharactersetLoader characterset_loader{
      configuration.charactersetFilesDirectory(),
      configuration.charactersetFilesPrefix(),
      configuration.charactersetFilesSuffix()};
  vector<Spriteset> charactersets{
      characterset_loader.load(configuration.charactersetsPath(), renderer)};

  unordered_map<int64_t, AnimationPlayer const> animation_players{
      AnimationPlayerLoader::load(configuration.animationsPath())};
  unordered_map<int64_t, ActionSpriteMapper const> action_sprite_mappers{
      ActionSpriteMapperLoader::load(configuration.actionSpriteMapsPath())};
  unordered_map<int64_t, TileSolidMapper const> tile_solid_mappers{
      TileSolidMapperLoader::load(configuration.tileSolidMapsPath())};
  unordered_map<int64_t, Solid const> solids{
      SolidLoader::load(configuration.solidsPath())};

  bool quit = false;
  SDL_Event event;
  KeyboardState keyboard_state;
  // FIXME: Hardcoded first mapper.
  Navigator navigator{tile_solid_mappers.at(0), solids, level,
                      tileset.spritesWidth(), tileset.spritesHeight()};
  auto [characters_to_controllers, characters]{CharacterLoader::load(
      configuration.charactersPath(), charactersets, action_sprite_mappers,
      animation_players, solids, navigator, tileset.spritesWidth(),
      tileset.spritesHeight())};
  vector<KeyboardCharacterController> character_controllers{
      CharacterControllerLoader::load(characters, characters_to_controllers)};
  int64_t tick(0);
  while (!quit) {
    renderer.render(animation_players.at(0), level, tileset, viewport, tick++,
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
    for (auto& controller : character_controllers) {
      controller.handleKeyboardEvent(keyboard_state);
    }
    // FIXME: Way to define which character is followed by the viewport.
    if (!character_controllers.empty()) {
      updateViewport(character_controllers[0].character(), viewport, level,
                     tileset.spritesWidth(), tileset.spritesHeight());
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
