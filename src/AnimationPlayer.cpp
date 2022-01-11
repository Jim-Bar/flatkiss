#include "AnimationPlayer.hpp"

AnimationPlayer::AnimationPlayer(std::unordered_map<uint16_t, Animation const>&& AnimationsPerTileIndex) {
    this->AnimationsPerTileIndex.swap(AnimationsPerTileIndex);
}

uint16_t AnimationPlayer::animatedTileIndexFor(uint16_t TileIndex, size_t Tick) const {
    if (AnimationsPerTileIndex.find(TileIndex) == AnimationsPerTileIndex.end()) { // FIXME: Use contains()
        return TileIndex;
    }

    Animation const& animation{AnimationsPerTileIndex.at(TileIndex)};
    return animation.tileIndexAtStep((Tick % (animation.getPeriod() * animation.getDuration())) / animation.getDuration());
}
