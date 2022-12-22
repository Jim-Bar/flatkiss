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

#include <SDL2/SDL.h>

#include <libflatkiss/media/event_handler.hpp>

void EventHandler::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      must_quit_ = true;
    } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
      updateKeysState(event);
    }
  }
}

bool EventHandler::isKeyPressed(Key key) const { return keys_state_[key]; }

bool EventHandler::mustQuit() const { return must_quit_; }

void EventHandler::updateKeysState(SDL_Event const& event) {
  SDL_Scancode key{event.key.keysym.scancode};
  bool pressed{event.key.state == SDL_PRESSED};
  switch (key) {  // FIXME: Makes a mapping array instead of a giant switch.
    case SDL_SCANCODE_UP:
      keys_state_[Key::kUp] = pressed;
      return;
    case SDL_SCANCODE_DOWN:
      keys_state_[Key::kDown] = pressed;
      return;
    case SDL_SCANCODE_LEFT:
      keys_state_[Key::kLeft] = pressed;
      return;
    case SDL_SCANCODE_RIGHT:
      keys_state_[Key::kRight] = pressed;
      return;
    default:
      return;  // FIXME: Exception.
  }
}
