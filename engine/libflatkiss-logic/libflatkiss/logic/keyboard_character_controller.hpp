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

#ifndef LIBFLATKISS_LOGIC_KEYBOARD_CHARACTER_CONTROLLER_HPP_INCLUDED
#define LIBFLATKISS_LOGIC_KEYBOARD_CHARACTER_CONTROLLER_HPP_INCLUDED

#include <libflatkiss/logic/character_controller.hpp>
#include <libflatkiss/logic/navigator.hpp>
#include <libflatkiss/media/media.hpp>
#include <libflatkiss/model/model.hpp>
#include <memory>
#include <vector>

class KeyboardCharacterController : public CharacterController {
 public:
  KeyboardCharacterController(Character& character);
  Character const& character() const;
  void onTick(int64_t tick, EventHandler const& event_handler,
              Navigator const& navigator, Level const& level);

 private:
  Character& character_;
  static int64_t constexpr kSpeedInPixels{1};
  int64_t const max_sidestep_distance_;
  int64_t sidestep_distance_{0};
};

#endif
