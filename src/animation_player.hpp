#ifndef ANIMATION_PLAYER_HPP_INCLUDED
#define ANIMATION_PLAYER_HPP_INCLUDED

#include <cstdint>
#include <unordered_map>

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
  uint16_t animatedTileIndexFor(uint16_t tile_index, size_t tick) const;

 private:
  std::unordered_map<uint16_t, Animation const> animations_per_tile_index_;
};

#endif
