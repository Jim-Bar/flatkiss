#ifndef KEYBOARD_STATE_HPP_INCLUDED
#define KEYBOARD_STATE_HPP_INCLUDED

#include <SDL2/SDL.h>

/**
 * @brief Holds the state of the keyboard.
 * 
 * Only a set of keys are implemented, refer to the implementation for more details. 
 */
class KeyboardState {
public:
    bool isPressed(SDL_Scancode Key) const;
    void update(SDL_Scancode Key, bool Pressed);

private:
    bool isDownPressed{false};
    bool isLeftPressed{false};
    bool isRightPressed{false};
    bool isUpPressed{false};
};

#endif
