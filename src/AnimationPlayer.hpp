#ifndef ANIMATION_PLAYER_HPP_INCLUDED
#define ANIMATION_PLAYER_HPP_INCLUDED

#include <cstdint>
#include <unordered_map>

#include "Animation.hpp"

class AnimationPlayer {
public:
    AnimationPlayer(std::unordered_map<uint16_t, Animation>& AnimationsPerTileIndex);
    uint16_t animatedTileIndexFor(uint16_t TileIndex, size_t Tick) const;

private:
    std::unordered_map<uint16_t, Animation> AnimationsPerTileIndex;
};

#endif
