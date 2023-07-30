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

#include <libflatkiss/logic/logic.hpp>

using std::vector;

Logic::Logic(
    std::vector<KeyboardCharacterController> const& character_controllers,
    Navigator const& navigator)
    : character_controllers_{character_controllers}, navigator_{navigator} {}

vector<KeyboardCharacterController>& Logic::characterControllers() {
  return character_controllers_;
}

Navigator const& Logic::navigator() const { return navigator_; }
