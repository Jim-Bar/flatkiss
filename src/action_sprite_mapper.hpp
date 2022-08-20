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

#ifndef ACTION_SPRITE_MAPPER_HPP_INCLUDED
#define ACTION_SPRITE_MAPPER_HPP_INCLUDED

#include <cstdint>
#include <string>
#include <unordered_map>

#include "action.hpp"

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
  ActionSpriteMapper(std::unordered_map<Action, uint16_t>&& action_to_indices);
  uint16_t spriteIndexForAction(Action const& action) const;

 private:
  std::unordered_map<Action, uint16_t> const action_to_indices_;
};

class ActionSpriteMapperLoader {
 public:
  static std::unordered_map<int64_t, ActionSpriteMapper const> load(
      std::string const& indices_file_path);

 private:
  static Action actionIdentifierToAction(uint16_t action_identifier);
  static std::unordered_map<Action, uint16_t> loadGroup(int64_t group_size,
                                                        std::ifstream& stream);
};

#endif
