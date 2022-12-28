/*
 * Copyright (C) 2021-2022 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Refer to 'COPYING.txt' for the full notice.
 */

#include <chrono>
#include <cstdlib>
#include <flatkiss/configuration.hpp>
#include <flatkiss/main.hpp>
#include <fstream>
#include <iostream>
#include <libflatkiss/logic/logic.hpp>
#include <libflatkiss/media/media.hpp>
#include <libflatkiss/model/model.hpp>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::move;
using std::unique_ptr;
using std::unordered_map;
using std::vector;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

int64_t const kCharacterSizePixels(16);
int64_t const kViewportSize(160);

void updateViewport(Character const& character, PositionedRectangle& viewport,
                    Level const& level, int64_t tiles_width,
                    int64_t tiles_height) {
  if (character.y() < viewport.rectangle().height() / 2 -
                          character.spriteset().spritesHeight() / 2) {
    viewport.y(0);
  } else if (character.y() > level.heightInTiles() * tiles_height -
                                 viewport.rectangle().height() / 2 -
                                 character.spriteset().spritesHeight() / 2) {
    viewport.y(level.heightInTiles() * tiles_height -
               viewport.rectangle().height());
  } else {
    viewport.y(character.y() - viewport.rectangle().height() / 2 +
               character.spriteset().spritesHeight() / 2);
  }
  if (character.x() < viewport.rectangle().width() / 2 -
                          character.spriteset().spritesWidth() / 2) {
    viewport.x(0);
  } else if (character.x() > level.widthInTiles() * tiles_width -
                                 viewport.rectangle().width() / 2 -
                                 character.spriteset().spritesWidth() / 2) {
    viewport.x(level.widthInTiles() * tiles_width -
               viewport.rectangle().width());
  } else {
    viewport.x(character.x() - viewport.rectangle().width() / 2 +
               character.spriteset().spritesWidth() / 2);
  }
}

int main(int argc, char* argv[]) {
  Configuration configuration{"configuration.ini"};

  PositionedRectangle viewport{Position{0, 0},
                               Rectangle{kViewportSize, kViewportSize}};
  Window const window{FLATKISS_PROJECT_NAME, viewport.rectangle().width(),
                      viewport.rectangle().height()};

  vector<Spriteset> const spritesets{
      SpritesetLoader::load(configuration.spritesetsPath())};
  TextureAtlas textures{spritesets, window.renderer(),
                        configuration.spritesetFilesDirectory(),
                        configuration.spritesetFilesPrefix(),
                        configuration.spritesetFilesSuffix()};

  unordered_map<int64_t, AnimationPlayer const> animation_players{
      AnimationPlayerLoader::load(configuration.animationsPath())};
  unordered_map<int64_t, ActionSpriteMapper const> action_sprite_mappers{
      ActionSpriteMapperLoader::load(configuration.actionSpriteMapsPath())};
  unordered_map<int64_t, TileSolidMapper const> tile_solid_mappers{
      TileSolidMapperLoader::load(configuration.tileSolidMapsPath())};
  unordered_map<int64_t, Solid const> solids{
      SolidLoader::load(configuration.solidsPath())};
  vector<Character> characters;
  auto [characters_to_controllers, character_templates]{
      CharacterLoader::load(configuration.charactersPath(), spritesets,
                            action_sprite_mappers, animation_players, solids)};
  vector<Level> const levels{LevelLoader::load(
      configuration.levelsPath(), spritesets, animation_players,
      tile_solid_mappers, character_templates, characters)};
  Level const& level{levels[0]};

  bool quit = false;
  vector<KeyboardCharacterController> character_controllers{
      CharacterControllerLoader::load(characters,
                                      characters_to_controllers)};
  int64_t tick(0);
  EventHandler event_handler;
  while (!quit) {
    window.render(level, viewport, tick++, textures, characters);
    sleep_for(milliseconds(configuration.engineTickDurationMs()));
    event_handler.handleEvents();
    quit = event_handler.mustQuit();
    for (auto& controller : character_controllers) {
      controller.handleKeyboardEvent(event_handler);
    }
    // FIXME: Way to define which character is followed by the viewport.
    if (!character_controllers.empty()) {
      updateViewport(character_controllers[0].character(), viewport, level,
                     level.spriteset().spritesWidth(),
                     level.spriteset().spritesHeight());
    }
  }

  return EXIT_SUCCESS;
}
