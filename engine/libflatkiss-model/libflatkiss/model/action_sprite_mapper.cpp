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
#include <utility>

using std::move;
using std::unordered_map;

ActionSpriteMapper::ActionSpriteMapper(
    unordered_map<Action, Sprite>&& action_to_indices)
    : action_to_indices_{move(action_to_indices)} {}

Sprite const& ActionSpriteMapper::spriteForAction(Action const& action) const {
  if (action_to_indices_.contains(action)) {
    return action_to_indices_.at(action);
  }

  return 0;  // FIXME: Raise exception.
}
