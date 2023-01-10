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

#ifndef LIBFLATKISS_MEDIA_WINDOW_HPP_INCLUDED
#define LIBFLATKISS_MEDIA_WINDOW_HPP_INCLUDED

#include <cstdint>
#include <libflatkiss/media/renderer.hpp>
#include <string>
#include <vector>

// Forward declaration to avoid exposing SDL to the outside world.
struct SDL_Window;

/**
 * @brief Represents a window shown to the user.
 */
class Window {
 public:
  Window(std::string display_name, int64_t width, int64_t height);
  Window(Window const& other) = delete;
  Window(Window&& other) = delete;
  Window& operator=(Window const& other) = delete;
  Window& operator=(Window&& other) = delete;
  ~Window();

  void render(Level const& level, PositionedRectangle const& viewport,
              int64_t tick, TextureAtlas const& textures,
              std::vector<Character> const& characters) const;
  Renderer const& renderer() const;

 private:
  SDL_Window* const sdl_window_;
  Renderer renderer_;
  static int64_t windows_count_;

  static SDL_Window* createSDLWindow(std::string display_name, int64_t width,
                                     int64_t height);
  static void initSDL();
  static void quitSDL();
};

#endif
