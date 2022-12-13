#include <SDL2/SDL.h>

#include <libflatkiss/media/event_handler.hpp>

void EventHandler::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      must_quit_ = true;
    } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
      updateKeysState(event.key.keysym.scancode,
                      event.key.state == SDL_PRESSED);
    }
  }
}

bool EventHandler::isKeyPressed(Key key) const { return keys_state_[key]; }

bool EventHandler::mustQuit() const { return must_quit_; }

void EventHandler::updateKeysState(SDL_Scancode key, bool pressed) {
  switch (key) {  // FIXME: Makes an mapping array instead of a giant switch.
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
