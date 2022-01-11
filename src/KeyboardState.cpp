#include "KeyboardState.hpp"

bool KeyboardState::isPressed(SDL_Scancode key) const {
    switch (key) {
        case SDL_SCANCODE_UP:
            return isUpPressed;
        case SDL_SCANCODE_DOWN:
            return isDownPressed;
        case SDL_SCANCODE_LEFT:
            return isLeftPressed;
        case SDL_SCANCODE_RIGHT:
            return isRightPressed;
    }

    return false; // FIXME: Exception.
}

void KeyboardState::update(SDL_Scancode Key, bool Pressed) {
    switch (Key) {
        case SDL_SCANCODE_UP:
            isUpPressed = Pressed;
            return;
        case SDL_SCANCODE_DOWN:
            isDownPressed = Pressed;
            return;
        case SDL_SCANCODE_LEFT:
            isLeftPressed = Pressed;
            return;
        case SDL_SCANCODE_RIGHT:
            isRightPressed = Pressed;
            return;
    }
}
