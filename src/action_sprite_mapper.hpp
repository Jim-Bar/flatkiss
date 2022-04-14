#ifndef ACTION_SPRITE_MAPPER_HPP_INCLUDED
#define ACTION_SPRITE_MAPPER_HPP_INCLUDED

#include <cstdint>
#include <string>
#include <unordered_map>

#include "action.hpp"

/**
 * @brief Maps actions to the index of the first sprite of the animation showing
 * that action in a characterset.
 *
 * Given an action, and when combined with a characterset, this class helps
 * identifying the first sprite showing that action.
 */
class ActionSpriteMapper {
 public:
  /**
   * @brief Construct a ActionSpriteMapper from a map of actions to matching sprite
   * indices.
   *
   * When this constructor returns, the passed map is empty because its values
   * have been transferred to the player. FIXME: No and compare with
   * AnimationPlayer.
   *
   * @param action_to_indices Map of actions to the index of the first sprite
   * showing this action in a characterset.
   */
  ActionSpriteMapper(std::unordered_map<Action, uint16_t>&& action_to_indices);
  uint16_t spriteIndexForAction(Action const& action) const;

 private:
  std::unordered_map<Action, uint16_t> const action_to_indices_;
};

class ActionSpriteMapperLoader {
 public:
  static std::unordered_map<int64_t, ActionSpriteMapper const> load(
      std::string const& indices_file_path);

 private:
  static Action actionIdentifierToAction(uint16_t action_identifier);
  static std::unordered_map<Action, uint16_t> loadGroup(int64_t group_size,
                                                        std::ifstream& stream);
};

#endif
