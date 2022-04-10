#ifndef ANIMATION_PLAYER_HPP_INCLUDED
#define ANIMATION_PLAYER_HPP_INCLUDED

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "animation.hpp"

/**
 * @brief Maps tile indices to their animated counterparts.
 *
 * Given a tile index and a tick, this class helps determining the index of the
 * tile which should be displayed.
 */
class AnimationPlayer {
 public:
  /**
   * @brief Construct an AnimationPlayer from a map of animations.
   *
   * When this constructor returns, the passed map is empty because its values
   * have been transferred to the player.
   *
   * @param animations_per_tile_index Map of tile indices to their respective
   * animations.
   */
  AnimationPlayer(std::unordered_map<uint16_t, Animation const>&&
                      animations_per_tile_index);
  uint16_t animatedTileIndexFor(uint16_t tile_index, int64_t tick) const;
  int64_t animationDurationForTileIndex(uint16_t tile_index) const;

 private:
  std::unordered_map<uint16_t, Animation const> animations_per_tile_index_;
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
