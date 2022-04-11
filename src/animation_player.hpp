#ifndef ANIMATION_PLAYER_HPP_INCLUDED
#define ANIMATION_PLAYER_HPP_INCLUDED

#include <cstdint>
#include <fstream>
#include <unordered_map>

#include "animation.hpp"

/**
 * @brief Maps sprite indices to their animated counterparts.
 *
 * Given a sprite index and a tick, this class helps determining the index of
 * the sprite which should be displayed.
 */
class AnimationPlayer {
 public:
  /**
   * @brief Construct an AnimationPlayer from a map of animations.
   *
   * When this constructor returns, the passed map is empty because its values
   * have been transferred to the player.
   *
   * @param animations_per_sprite_index Map of sprite indices to their
   * respective animations.
   */
  AnimationPlayer(std::unordered_map<uint16_t, Animation const>&&
                      animations_per_sprite_index);
  uint16_t animatedSpriteIndexFor(uint16_t sprite_index, int64_t tick) const;
  int64_t animationDurationForSpriteIndex(uint16_t sprite_index) const;

 private:
  std::unordered_map<uint16_t, Animation const> animations_per_sprite_index_;
};

/**
 * @brief Helper class for loading the animations from a file.
 */
class AnimationPlayerLoader {
 public:
  static std::unordered_map<int64_t, AnimationPlayer const> load(
      std::string const& file_path);

 private:
  static std::unordered_map<uint16_t, Animation const> loadGroup(
      uint16_t group_size, std::ifstream& animations_stream);
};

#endif
