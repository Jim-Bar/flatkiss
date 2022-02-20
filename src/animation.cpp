#include "animation.hpp"

#include <fstream>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::piecewise_construct;
using std::string;
using std::unordered_map;
using std::vector;

Animation::Animation(vector<uint16_t> const& tile_indices, uint8_t period,
                     uint8_t duration)
    : tile_indices_(move(tile_indices)), period_(period), duration_(duration) {}

uint8_t Animation::getDuration() const { return duration_; }

uint8_t Animation::getPeriod() const { return period_; }

uint16_t Animation::tileIndexAtStep(uint16_t step) const {
  return tile_indices_[step];
}

unordered_map<uint16_t, Animation const> AnimationLoader::load(
    string const& file_path) {
  ifstream stream;
  unordered_map<uint16_t, Animation const> animations_per_tile_index;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      uint8_t animation_period{static_cast<uint8_t>(stream.get())};
      uint8_t animation_duration{static_cast<uint8_t>(stream.get())};
      /* The vector containing animations is created and space is reserved for
       * containing all of them at the same time. Then the stream is read
       * directly into the vector. */
      auto animation{vector<uint16_t>(animation_period, 0)};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(animation.data()),
                  animation_period * 2);  // Two bytes per tile.
      animations_per_tile_index.emplace(
          piecewise_construct, forward_as_tuple(animation[0]),
          forward_as_tuple(move(animation), animation_period,
                           animation_duration));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return animations_per_tile_index;
}
