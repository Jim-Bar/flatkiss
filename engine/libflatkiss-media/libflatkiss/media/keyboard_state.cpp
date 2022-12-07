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

#include <libflatkiss/media/keyboard_state.hpp>

bool KeyboardState::isPressed(SDL_Scancode key) const {
  switch (key) {
    case SDL_SCANCODE_UP:
      return is_up_pressed_;
    case SDL_SCANCODE_DOWN:
      return is_down_pressed_;
    case SDL_SCANCODE_LEFT:
      return is_left_pressed_;
    case SDL_SCANCODE_RIGHT:
      return is_right_pressed_;
    default:
      return false;  // FIXME: Exception.
  }
}

void KeyboardState::update(SDL_Scancode key, bool pressed) {
  switch (key) {
    case SDL_SCANCODE_UP:
      is_up_pressed_ = pressed;
      return;
    case SDL_SCANCODE_DOWN:
      is_down_pressed_ = pressed;
      return;
    case SDL_SCANCODE_LEFT:
      is_left_pressed_ = pressed;
      return;
    case SDL_SCANCODE_RIGHT:
      is_right_pressed_ = pressed;
      return;
    default:
      return;  // FIXME: Exception.
  }
}
