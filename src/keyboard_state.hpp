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

#ifndef KEYBOARD_STATE_HPP_INCLUDED
#define KEYBOARD_STATE_HPP_INCLUDED

#include <SDL2/SDL.h>

/**
 * @brief Holds the state of the keyboard.
 *
 * Only a set of keys are implemented, refer to the implementation for more
 * details.
 */
class KeyboardState {
 public:
  bool isPressed(SDL_Scancode key) const;
  void update(SDL_Scancode key, bool pressed);

 private:
  bool is_down_pressed_{false};
  bool is_left_pressed_{false};
  bool is_right_pressed_{false};
  bool is_up_pressed_{false};
};

#endif
