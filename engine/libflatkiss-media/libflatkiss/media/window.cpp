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

#include <algorithm>
#include <iostream>
#include <libflatkiss/media/window.hpp>

using std::cerr;
using std::endl;
using std::max;
using std::string;

// Lint disabled because of this bug: https://stackoverflow.com/a/64680981
// NOLINTNEXTLINE
int64_t Window::windows_count_{0};

Window::Window(string display_name, int64_t width, int64_t height)
    : sdl_window_{Window::createSDLWindow(display_name, width, height)},
      renderer_{sdl_window_} {}

Window::~Window() {
  SDL_DestroyWindow(sdl_window_);

  if (windows_count_ == 1) {
    quitSDL();
  }

  windows_count_ = max(static_cast<int64_t>(0), windows_count_ - 1);
}

SDL_Window* Window::createSDLWindow(string display_name, int64_t width,
                                    int64_t height) {
  if (windows_count_ == 0) {
    initSDL();
  }

  SDL_Window* sdl_window{SDL_CreateWindow(
      display_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      static_cast<int>(width), static_cast<int>(height), SDL_WINDOW_SHOWN)};
  if (sdl_window == nullptr) {
    cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
    quitSDL();
    return nullptr;  // FIXME: Raise an exception.
  }

  windows_count_++;
  return sdl_window;
}

void Window::initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    cerr << "SDL_Init Error: " << SDL_GetError() << endl;
    // FIXME: Raise an exception.
  }
}

void Window::quitSDL() { SDL_Quit(); }

void Window::render(Level const& level, PositionedRectangle const& viewport,
                    int64_t tick, TextureAtlas const& textures,
                    std::vector<Character> const& characters) const {
  renderer_.render(level, viewport, tick++, textures, characters);
}

Renderer const& Window::renderer() const { return renderer_; }
