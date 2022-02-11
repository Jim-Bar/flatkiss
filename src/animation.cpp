#include "animation.hpp"

#include <fstream>

Animation::Animation(std::vector<uint16_t> const& tile_indices, uint8_t period,
                     uint8_t duration)
    : tile_indices_(std::move(tile_indices)),
      period_(period),
      duration_(duration) {}

uint8_t Animation::getDuration() const { return duration_; }

uint8_t Animation::getPeriod() const { return period_; }

uint16_t Animation::tileIndexAtStep(uint16_t step) const {
  return tile_indices_[step];
}

std::unordered_map<uint16_t, Animation const> AnimationLoader::load(
    std::string const& file_path) {
  std::ifstream stream;
  std::unordered_map<uint16_t, Animation const> animations_per_tile_index;
  stream.open(file_path, std::ios::in | std::ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != std::istream::traits_type::eof()) {
      uint8_t animation_period{static_cast<uint8_t>(stream.get())};
      uint8_t animation_duration{static_cast<uint8_t>(stream.get())};
      /* The vector containing animations is created and space is reserved for
       * containing all of them at the same time. Then the stream is read
       * directly into the vector. */
      auto animation{std::vector<uint16_t>(animation_period, 0)};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(animation.data()),
                  animation_period * 2);  // Two bytes per tile.
      animations_per_tile_index.emplace(
          std::piecewise_construct, std::forward_as_tuple(animation[0]),
          std::forward_as_tuple(std::move(animation), animation_period,
                                animation_duration));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return animations_per_tile_index;
}
