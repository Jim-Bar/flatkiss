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

#include <iostream>
#include <libflatkiss/media/window.hpp>

using std::cerr;
using std::endl;
using std::string;

Window::Window(string display_name, int64_t width, int64_t height)
    : sdl_window_{SDL_CreateWindow(
          display_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED, static_cast<int>(width),
          static_cast<int>(height), SDL_WINDOW_SHOWN)},
      renderer_{sdl_window_} {
  if (sdl_window_ == nullptr) {
    cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
    // FIXME: Raise an exception.
  }
}

Window::~Window() { SDL_DestroyWindow(sdl_window_); }

void Window::render(Level const& level, PositionedRectangle const& viewport,
                    int64_t tick, TextureAtlas const& textures,
                    std::vector<Character> const& characters) const {
  renderer_.render(level, viewport, tick++, textures, characters);
}

Renderer const& Window::renderer() const {
    return renderer_;
}
