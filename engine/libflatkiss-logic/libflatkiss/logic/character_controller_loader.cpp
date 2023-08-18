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

#include <libflatkiss/logic/character_controller_loader.hpp>
#include <libflatkiss/logic/keyboard_character_controller.hpp>
#include <libflatkiss/logic/stroll_character_controller.hpp>
#include <stdexcept>

using std::invalid_argument;
using std::make_unique;
using std::to_string;
using std::unique_ptr;
using std::vector;

void CharacterControllerLoader::load(
    vector<Character>& characters,
    vector<unique_ptr<CharacterController>>& into) {
  for (int64_t i{0}; i < characters.size(); i++) {
    switch (characters[i].controllers()[0]) {
      case ControllerType::kKeyboardController:
        into.push_back(make_unique<KeyboardCharacterController>(characters[i]));
        break;
      case ControllerType::kStrollController:
        into.push_back(make_unique<StrollCharacterController>(characters[i]));
        break;
      default:
        throw invalid_argument("Unknown controller type");
    }
  }
}
