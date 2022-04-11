#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

#include <cstdint>
#include <vector>

/**
 * @brief A list of sprite indices making up an animated sprite.
 *
 * An animation is a list of sprites displayed one after another. The period of
 * the animation is the number of sprites making it up. The duration is the
 * number of ticks a particular sprite is displayed before showing the next one.
 */
class Animation {
 public:
  Animation(std::vector<uint16_t> const& sprite_indices, uint8_t period,
            uint8_t Duration);
  int64_t getDuration() const;
  int64_t getPeriod() const;
  uint16_t spriteIndexAtStep(int64_t step) const;

 private:
  uint8_t const duration_;
  uint8_t const period_;
  std::vector<uint16_t> const sprite_indices_;
};

#endif
