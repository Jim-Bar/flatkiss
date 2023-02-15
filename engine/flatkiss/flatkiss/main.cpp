/*
 * Copyright (C) 2021-2023 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
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
#include <libflatkiss/data/data.hpp>
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
  Collider collider{};
  cout << "1 " << collider.collide(PositionedEllipse{Position{0, 0}, Ellipse{1, 1}}, PositionedEllipse{Position{1, 0}, Ellipse{1, 1}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{0, 0}, Ellipse{1, 1}}, PositionedEllipse{Position{3, 0}, Ellipse{1, 1}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{-150, 100}, Ellipse{112, 50}}, PositionedEllipse{Position{100, -50}, Ellipse{112, 100}}) << endl;
  cout << "1 " << collider.collide(PositionedEllipse{Position{-50, 0}, Ellipse{112, 50}}, PositionedEllipse{Position{100, -50}, Ellipse{112, 100}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{-50, -150}, Ellipse{71, 50}}, PositionedEllipse{Position{100, -50}, Ellipse{112, 100}}) << endl;
  cout << "1 " << collider.collide(PositionedEllipse{Position{-50, -150}, Ellipse{71, 50}}, PositionedEllipse{Position{100, -100}, Ellipse{112, 100}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{-100, 0}, Ellipse{50, 354}}, PositionedEllipse{Position{300, -300}, Ellipse{364, 100}}) << endl;
  cout << "1 " << collider.collide(PositionedEllipse{Position{-100, 0}, Ellipse{50, 354}}, PositionedEllipse{Position{300, -300}, Ellipse{403, 200}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{-150, 0}, Ellipse{100, 141}}, PositionedEllipse{Position{150, 100}, Ellipse{212, 150}}) << endl;
  cout << "1 " << collider.collide(PositionedEllipse{Position{-150, 50}, Ellipse{100, 180}}, PositionedEllipse{Position{150, 100}, Ellipse{212, 150}}) << endl;
  cout << "1 " << collider.collide(PositionedEllipse{Position{100, 100}, Ellipse{50, 112}}, PositionedEllipse{Position{150, 100}, Ellipse{212, 150}}) << endl;
  cout << "1 " << collider.collide(PositionedEllipse{Position{100, 50}, Ellipse{50, 112}}, PositionedEllipse{Position{150, 100}, Ellipse{212, 150}}) << endl;
  cout << "1 " << collider.collide(PositionedEllipse{Position{-400, 0}, Ellipse{10, 600}}, PositionedEllipse{Position{0, 100}, Ellipse{600, 10}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{-620, -520}, Ellipse{10, 600}}, PositionedEllipse{Position{0, 100}, Ellipse{600, 10}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{-400, -200}, Ellipse{200, 633}}, PositionedEllipse{Position{200, 0}, Ellipse{400, 721}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{-800, -200}, Ellipse{600, 849}}, PositionedEllipse{Position{800, 0}, Ellipse{1000, 1166}}) << endl;
  cout << "1 " << collider.collide(PositionedEllipse{Position{-800, -200}, Ellipse{627, 868}}, PositionedEllipse{Position{800, 0}, Ellipse{1000, 1166}}) << endl;
  cout << "0 " << collider.collide(PositionedEllipse{Position{100, 200}, Ellipse{403, 350}}, PositionedEllipse{Position{425, -100}, Ellipse{34, 22}}) << endl;
  /*Configuration configuration{"configuration.ini"};

  PositionedRectangle viewport{Position{0, 0},
                               Rectangle{kViewportSize, kViewportSize}};
  Window const window{FLATKISS_PROJECT_NAME, viewport.rectangle().width(),
                      viewport.rectangle().height()};  

  Data data{
      configuration.actionSpriteMapsPath(), configuration.animationsPath(),
      configuration.charactersPath(),       configuration.levelsPath(),
      configuration.solidsPath(),           configuration.spritesetsPath(),
      configuration.tileSolidMapsPath()};
  Model model{data.load()};

  Navigator navigator{model.solids()};
  vector<KeyboardCharacterController> character_controllers;
  for (auto& level : model.levels()) {
    for (auto& character :
         CharacterControllerLoader::load(level.characters())) {
      character_controllers.emplace_back(character);
    }
  }

  Logic logic{character_controllers, navigator};

  Level& level{model.levels()[0]};

  TextureAtlas textures{model.spritesets(), window.renderer(),
                        configuration.spritesetFilesDirectory(),
                        configuration.spritesetFilesPrefix(),
                        configuration.spritesetFilesSuffix()};

  bool quit = false;
  int64_t tick(0);
  EventHandler event_handler;
  while (!quit) {
    window.render(level, viewport, tick++, textures, level.characters());
    sleep_for(milliseconds(configuration.engineTickDurationMs()));
    event_handler.handleEvents();
    quit = event_handler.mustQuit();
    for (auto& controller : logic.character_controllers()) {
      // FIXME: Move all this into logic.
      controller.handleKeyboardEvent(event_handler, logic.navigator(), level);
    }
    // FIXME: Way to define which character is followed by the viewport.
    if (!logic.character_controllers().empty()) {
      updateViewport(level.characters()[0], viewport,
                     level, level.spriteset().spritesWidth(),
                     level.spriteset().spritesHeight());
    }
  }*/

  return EXIT_SUCCESS;
}
