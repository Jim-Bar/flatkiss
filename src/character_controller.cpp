#include "character_controller.hpp"

using std::vector;

KeyboardCharacterController::KeyboardCharacterController(Character& character)
    : character_{character} {}

Character const& KeyboardCharacterController::character() const {
  return character_;
}

void KeyboardCharacterController::handleKeyboardEvent(
    KeyboardState const& keyboard_state) {
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

vector<KeyboardCharacterController> CharacterControllerLoader::load(
    vector<Character>& characters, vector<int64_t> characters_to_controllers) {
  vector<KeyboardCharacterController> controllers{};
  for (int64_t i{0}; i < characters.size(); i++) {
    switch (characters_to_controllers[i]) {
      case 0:
        controllers.emplace_back(characters[i]);
        break;
      default:
        // FIXME: Raise exception.
        break;
    }
  }

  return controllers;
}
