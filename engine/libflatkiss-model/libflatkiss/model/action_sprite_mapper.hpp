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

#ifndef LIBFLATKISS_MODEL_SPRITE_MAPPER_HPP_INCLUDED
#define LIBFLATKISS_MODEL_SPRITE_MAPPER_HPP_INCLUDED

#include <cstdint>
#include <libflatkiss/model/action.hpp>
#include <libflatkiss/model/sprite.hpp>
#include <string>
#include <unordered_map>

/**
 * @brief Maps actions to the index of the first sprite of the animation showing
 * that action in a spriteset.
 *
 * Given an action, and when combined with a spriteset, this class helps
 * identifying the first sprite showing that action.
 */
class ActionSpriteMapper {
 public:
  /**
   * @brief Construct a ActionSpriteMapper from a map of actions to matching
   * sprite indices.
   *
   * @param action_to_indices Map of actions to the index of the first sprite
   * showing this action in a spriteset.
   */
  ActionSpriteMapper(std::unordered_map<Action, Sprite>&& action_to_indices);
  Sprite const& spriteForAction(Action const& action) const;

 private:
  std::unordered_map<Action, Sprite> const action_to_indices_;
};

#endif
