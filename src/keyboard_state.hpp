#ifndef KEYBOARD_STATE_HPP_INCLUDED
#define KEYBOARD_STATE_HPP_INCLUDED

#include <SDL2/SDL.h>

/**
 * @brief Holds the state of the keyboard.
 *
 * Only a set of keys are implemented, refer to the implementation for more
 * details.
 */
class KeyboardState {
 public:
  bool isPressed(SDL_Scancode key) const;
  void update(SDL_Scancode key, bool pressed);

 private:
  bool is_down_pressed_{false};
  bool is_left_pressed_{false};
  bool is_right_pressed_{false};
  bool is_up_pressed_{false};
};

#endif
