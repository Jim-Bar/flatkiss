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

#include <fstream>
#include <libflatkiss/model/action_sprite_mapper.hpp>
#include <stdexcept>
#include <string>
#include <utility>

using std::invalid_argument;
using std::move;
using std::to_string;
using std::unordered_map;

ActionSpriteMapper::ActionSpriteMapper(
    unordered_map<Action, uint16_t>&& action_to_indices)
    : action_to_indices_{move(action_to_indices)} {}

uint16_t ActionSpriteMapper::spriteIndexForAction(Action const& action) const {
  if (action_to_indices_.contains(action)) {
    return action_to_indices_.at(action);
  }

  throw invalid_argument("No sprite for action: " + to_string(action));
}
