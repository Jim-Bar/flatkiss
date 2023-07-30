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

#include <functional>
#include <libflatkiss/logic/stroll_character_controller.hpp>
#include <random>
#include <utility>

using std::bind;
using std::default_random_engine;
using std::move;
using std::uniform_int_distribution;

StrollCharacterController::StrollCharacterController(Character& character)
    : character_{character} {}

Character const& StrollCharacterController::character() const {
  return character_;
}

void StrollCharacterController::onTick(int64_t tick,
                                       EventHandler const& event_handler,
                                       Navigator const& navigator,
                                       Level const& level) {
  // At the beginning of a new cycle, decide on a random direction.
  if (tick % (kIdleTimeInTicks + kWalkTimeInTicks) == 0) {
    switch (randomValue(1, 4)) {
      case 1:
        current_direction_ = kSouth;
        break;
      case 2:
        current_direction_ = kNorth;
        break;
      case 3:
        current_direction_ = kWest;
        break;
      case 4:
        current_direction_ = kEast;
        break;
    }
  }

  Vector movement{
      current_direction_ == kWest ? -1 : (current_direction_ == kEast ? 1 : 0),
      current_direction_ == kNorth ? -1
                                   : (current_direction_ == kSouth ? 1 : 0)};

  if (tick % (kIdleTimeInTicks + kWalkTimeInTicks) < kWalkTimeInTicks) {
    // Walking time.
    auto [_, final_position]{navigator.moveBy(character_.positionedSolid(),
                                              movement, level, 0,
                                              kSpeedInPixels, true)};
    character_.updateFacingDirection(movement,
                                     final_position - character_.position());
    character_.moveTo(move(final_position));
  } else {
    // Idle time. Reset the animation when the character is not moving.
    character_.updateFacingDirection(Vector::kZero, Vector::kZero);
  }
}

int64_t StrollCharacterController::randomValue(int64_t lower, int64_t upper) {
  static auto gen = bind(uniform_int_distribution<int64_t>(lower, upper),
                         default_random_engine());
  return gen();
}
