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

#include <libflatkiss/logic/character_controller.hpp>

using std::vector;

KeyboardCharacterController::KeyboardCharacterController(Character& character)
    : character_{character} {}

Character const& KeyboardCharacterController::character() const {
  return character_;
}

void KeyboardCharacterController::handleKeyboardEvent(
    EventHandler const& event_handler) {
  int64_t dx{0};
  int64_t dy{0};
  if (event_handler.isKeyPressed(Key::kUp)) {
    dy -= kSpeedInPixels;
  }
  if (event_handler.isKeyPressed(Key::kDown)) {
    dy += kSpeedInPixels;
  }
  if (event_handler.isKeyPressed(Key::kLeft)) {
    dx -= kSpeedInPixels;
  }
  if (event_handler.isKeyPressed(Key::kRight)) {
    dx += kSpeedInPixels;
  }

  character_.moveBy(Vector{dx, dy});
}

vector<KeyboardCharacterController> CharacterControllerLoader::load(
    vector<Character>& characters, vector<int64_t> characters_to_controllers) {
  vector<KeyboardCharacterController> controllers{};
  for (int64_t i{0}; i < characters.size(); i++) {
    switch (characters_to_controllers[i]) {
      case 0:
        controllers.emplace_back(characters[i]);
        break;
      default:
        // FIXME: Raise exception.
        break;
    }
  }

  return controllers;
}
