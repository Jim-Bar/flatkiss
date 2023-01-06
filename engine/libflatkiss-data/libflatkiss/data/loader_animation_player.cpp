/*
 * Copyright (C) 2021-2022 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Refer to 'COPYING.txt' for the full notice.
 */

#include <fstream>
#include <libflatkiss/data/loader_animation_player.hpp>
#include <utility>
#include <vector>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::piecewise_construct;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

struct DataGroupInfo {
  uint16_t index;
  uint16_t size;
};

struct DataAnimation {
  uint8_t period;
  uint8_t duration;
};

unordered_map<int64_t, AnimationPlayer const> LoaderAnimationPlayer::load(
    string const& file_path) {
  unordered_map<int64_t, AnimationPlayer const> animation_players_per_group;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      DataGroupInfo group_info{};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&group_info), sizeof(DataGroupInfo));
      animation_players_per_group.emplace(
          piecewise_construct, forward_as_tuple(group_info.index),
          forward_as_tuple(move(loadGroup(group_info.size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return animation_players_per_group;
}

unordered_map<uint16_t, Animation> LoaderAnimationPlayer::loadGroup(
    int64_t group_size, ifstream& stream) {
  unordered_map<uint16_t, Animation> animations_per_sprite_index;
  for (int64_t i{0}; i < group_size; i++) {
    DataAnimation animation{};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(&animation), sizeof(DataAnimation));
    /* The vector containing animations is created and space is reserved for
     * containing all of them at the same time. Then the stream is read
     * directly into the vector. */
    vector<uint16_t> sprites(animation.period, 0);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(sprites.data()),
                animation.period * 2);  // Two bytes per sprite.
    animations_per_sprite_index.emplace(
        piecewise_construct, forward_as_tuple(sprites[0]),
        forward_as_tuple(move(sprites), animation.period, animation.duration));
  }

  return animations_per_sprite_index;
}
