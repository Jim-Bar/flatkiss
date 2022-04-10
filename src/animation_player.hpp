#ifndef ANIMATION_PLAYER_HPP_INCLUDED
#define ANIMATION_PLAYER_HPP_INCLUDED

#include <cstdint>
#include <unordered_map>
#include <vector>

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
  AnimationPlayerLoader(int64_t count, std::string animations_files_directory,
                        std::string animations_files_prefix,
                        std::string animations_files_suffix);
  std::vector<AnimationPlayer> load();

 private:
  std::string const animations_files_directory_;
  std::string const animations_files_prefix_;
  std::string const animations_files_suffix_;
  int64_t const count_;

  std::string animationsFilePath(int64_t index) const;
};

#endif
