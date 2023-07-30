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

void StrollCharacterController::onTick(int64_t tick, Navigator const& navigator,
                                       Level const& level) {
  if (tick % 10 < 5) {  // FIXME: "Variabilize" 10 and 5.
    Vector desired_displacement{randomValue(-1, 1) * kSpeedInPixels,
                                randomValue(-1, 1) * kSpeedInPixels};
    auto [_, final_position]{navigator.moveBy(character_.positionedSolid(),
                                              desired_displacement, level, 0,
                                              kSpeedInPixels, true)};
    character_.updateFacingDirection(desired_displacement,
                                     final_position - character_.position());
    character_.moveTo(move(final_position));
  }
}

int64_t StrollCharacterController::randomValue(int64_t lower, int64_t upper) {
  static auto gen = bind(uniform_int_distribution<int64_t>(lower, upper),
                         default_random_engine());
  return gen();
}
