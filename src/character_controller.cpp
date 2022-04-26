#include "character_controller.hpp"

KeyboardCharacterController::KeyboardCharacterController(Character& character)
    : character_{character} {}

Character const& KeyboardCharacterController::character() const {
  return character_;
}

void KeyboardCharacterController::handleKeyboardEvent(
    KeyboardState const& keyboard_state, std::unique_ptr<Level const>& level) {
  int64_t dx{0};
  int64_t dy{0};
  if (keyboard_state.isPressed(SDL_SCANCODE_UP)) {
    dy -= kSpeedInPixels;
  }
  if (keyboard_state.isPressed(SDL_SCANCODE_DOWN)) {
    dy += kSpeedInPixels;
  }
  if (keyboard_state.isPressed(SDL_SCANCODE_LEFT)) {
    dx -= kSpeedInPixels;
  }
  if (keyboard_state.isPressed(SDL_SCANCODE_RIGHT)) {
    dx += kSpeedInPixels;
  }

  character_.moveBy(Vector{dx, dy});
}
