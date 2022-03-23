#include "animation_player.hpp"

using std::unordered_map;

AnimationPlayer::AnimationPlayer(
    unordered_map<uint16_t, Animation const>&& animations_per_tile_index) {
  this->animations_per_tile_index_.swap(animations_per_tile_index);
}

uint16_t AnimationPlayer::animatedTileIndexFor(uint16_t tile_index,
                                               int64_t tick) const {
  if (!animations_per_tile_index_.contains(tile_index)) {
    return tile_index;
  }

  Animation const& animation{animations_per_tile_index_.at(tile_index)};
  return animation.tileIndexAtStep(
      (tick % (animation.getPeriod() * animation.getDuration())) /
      animation.getDuration());
}
