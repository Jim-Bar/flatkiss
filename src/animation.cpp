#include "animation.hpp"

using std::vector;

Animation::Animation(vector<uint16_t> const& sprite_indices, uint8_t period,
                     uint8_t duration)
    : sprite_indices_{move(sprite_indices)},
      period_{period},
      duration_{duration} {}

uint8_t Animation::getDuration() const { return duration_; }

uint8_t Animation::getPeriod() const { return period_; }

uint16_t Animation::spriteIndexAtStep(uint16_t step) const {
  return sprite_indices_[step];
}
