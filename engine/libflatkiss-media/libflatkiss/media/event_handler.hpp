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

#ifndef LIBFLATKISS_MEDIA_EVENT_HANDLER_HPP_INCLUDED
#define LIBFLATKISS_MEDIA_EVENT_HANDLER_HPP_INCLUDED

#include <libflatkiss/media/key.hpp>

// Forward declaration to avoid exposing SDL to the outside world.
// enum SDL_Scancode : int;
// FIXME: Forward declaration on an enum not tagged... Complicated:
// https://stackoverflow.com/a/1280969
// https://stackoverflow.com/q/22386191
// https://github.com/libsdl-org/SDL/blob/main/include/SDL3/SDL_scancode.h
#include <SDL2/SDL.h> // FIXME: Rhaaaaaa....

class EventHandler {
 public:
  void handleEvents();
  bool isKeyPressed(Key key) const;
  bool mustQuit() const;

 private:
  bool keys_state_[Key::kMax];
  bool must_quit_ = false;

  void updateKeysState(SDL_Scancode key, bool pressed);
};

#endif
