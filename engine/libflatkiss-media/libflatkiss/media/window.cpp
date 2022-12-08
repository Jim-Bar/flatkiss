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
