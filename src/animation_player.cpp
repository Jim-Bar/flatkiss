#include "animation_player.hpp"

AnimationPlayer::AnimationPlayer(
    std::unordered_map<uint16_t, Animation const>&& AnimationsPerTileIndex) {
  this->AnimationsPerTileIndex.swap(AnimationsPerTileIndex);
}

uint16_t AnimationPlayer::animatedTileIndexFor(uint16_t TileIndex,
                                               size_t Tick) const {
  if (!AnimationsPerTileIndex.contains(TileIndex)) {
    return TileIndex;
  }

  Animation const& animation{AnimationsPerTileIndex.at(TileIndex)};
  return animation.tileIndexAtStep(
      (Tick % (animation.getPeriod() * animation.getDuration())) /
      animation.getDuration());
}
