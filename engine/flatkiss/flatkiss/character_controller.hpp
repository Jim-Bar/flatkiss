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

#ifndef CHARACTER_CONTROLLER_HPP_INCLUDED
#define CHARACTER_CONTROLLER_HPP_INCLUDED

#include <memory>
#include <vector>

#include "media.hpp"
#include "model.hpp"

class KeyboardCharacterController {
 public:
  KeyboardCharacterController(Character& character);
  Character const& character() const;
  void handleKeyboardEvent(KeyboardState const& keyboard_state);

 private:
  Character& character_;
  static int64_t constexpr kSpeedInPixels{1};
};

class CharacterControllerLoader {
 public:
  static std::vector<KeyboardCharacterController> load(
      std::vector<Character>& characters,
      std::vector<int64_t> characters_to_controllers);
};

#endif
