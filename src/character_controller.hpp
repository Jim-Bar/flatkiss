#ifndef CHARACTER_CONTROLLER_HPP_INCLUDED
#define CHARACTER_CONTROLLER_HPP_INCLUDED

#include <memory>
#include <vector>

#include "character.hpp"
#include "keyboard_state.hpp"

class KeyboardCharacterController {
 public:
  KeyboardCharacterController(Character& character);
  Character const& character() const;
  void handleKeyboardEvent(KeyboardState const& keyboard_state);

 private:
  Character& character_;
  static int64_t constexpr kSpeedInPixels{1};
};

class CharacterControllerLoader {
 public:
  static std::vector<KeyboardCharacterController> load(
      std::vector<Character>& characters,
      std::vector<int64_t> characters_to_controllers);
};

#endif
