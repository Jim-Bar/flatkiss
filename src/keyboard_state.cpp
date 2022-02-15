#include "keyboard_state.hpp"

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
