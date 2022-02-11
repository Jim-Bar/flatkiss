#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief A list of tile indices making up an animated tile.
 *
 * An animation is a list of tiles displayed one after another. The period of
 * the animation is the number of tiles making it up. The duration is the number
 * of ticks a particular tile is displayed before showing the next one.
 */
class Animation {
 public:
  Animation(std::vector<uint16_t> const& tile_indices, uint8_t period,
            uint8_t Duration);
  uint8_t getDuration() const;
  uint8_t getPeriod() const;
  uint16_t tileIndexAtStep(uint16_t Step) const;

 private:
  uint8_t const duration_;
  uint8_t const period_;
  std::vector<uint16_t> const tile_indices_;
};

/**
 * @brief Helper class for loading the animations from a file.
 */
class AnimationLoader {
 public:
  static std::unordered_map<uint16_t, Animation const> load(
      std::string const& file_path);
};

#endif
