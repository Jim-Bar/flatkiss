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

#include <algorithm>
#include <libflatkiss/logic/character_controller.hpp>
#include <utility>

using std::min;
using std::move;
using std::vector;

KeyboardCharacterController::KeyboardCharacterController(Character& character)
    : character_{character},
      max_sidestep_distance_{character.positionedSolid().boundingBox().width() /
                             2} {}

Character const& KeyboardCharacterController::character() const {
  return character_;
}

void KeyboardCharacterController::handleKeyboardEvent(
    EventHandler const& event_handler, Navigator const& navigator,
    Level const& level) {
  int64_t delta_x{0};
  int64_t delta_y{0};
  if (event_handler.isKeyPressed(Key::kUp)) {
    delta_y -= kSpeedInPixels;
  }
  if (event_handler.isKeyPressed(Key::kDown)) {
    delta_y += kSpeedInPixels;
  }
  if (event_handler.isKeyPressed(Key::kLeft)) {
    delta_x -= kSpeedInPixels;
  }
  if (event_handler.isKeyPressed(Key::kRight)) {
    delta_x += kSpeedInPixels;
  }
  Vector desired_displacement{delta_x, delta_y};

  /* Each tick, increase the sidestep lookup distance by one. This causes the
   * character to slow down when side-stepping (compared to looking up the
   * maximum distance right away). */
  sidestep_distance_ = min(max_sidestep_distance_, sidestep_distance_ + 1);
  auto [side_stepped, final_position]{
      navigator.moveBy(character_.positionedSolid(), desired_displacement,
                       level, sidestep_distance_, kSpeedInPixels, true)};
  if (final_position != character_.position()) {
    /* In case of side-stepping, this resets the lookup from start, causing the
     * slow down to be gradual with the distance to the final side-step. When
     * not side-stepping, this has no effect. */
    sidestep_distance_ = 0;
  }

  character_.updateFacingDirection(
      // When side-stepping, do as if the character were not changing direction.
      desired_displacement, side_stepped
                                ? desired_displacement
                                : final_position - character_.position());
  character_.moveTo(move(final_position));
}

vector<KeyboardCharacterController> CharacterControllerLoader::load(
    vector<Character>& characters) {
  vector<KeyboardCharacterController> controllers{};
  for (int64_t i{0}; i < characters.size(); i++) {
    switch (characters[i].controllers()[0]) {
      case ControllerType::kKeyboardController:
        controllers.emplace_back(characters[i]);
        break;
      default:
        // FIXME: Raise exception.
        break;
    }
  }

  return controllers;
}
