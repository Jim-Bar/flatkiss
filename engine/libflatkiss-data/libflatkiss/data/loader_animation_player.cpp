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

unordered_map<int64_t, AnimationPlayer const> LoaderAnimationPlayer::load(
    string const& file_path) {
  unordered_map<int64_t, AnimationPlayer const> animation_players_per_group;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      uint16_t group_index{0};
      uint16_t group_size{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&group_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&group_size), 2);
      animation_players_per_group.emplace(
          piecewise_construct, forward_as_tuple(group_index),
          forward_as_tuple(move(loadGroup(group_size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return animation_players_per_group;
}

unordered_map<uint16_t, Animation> LoaderAnimationPlayer::loadGroup(
    int64_t group_size, ifstream& stream) {
  unordered_map<uint16_t, Animation> animations_per_sprite_index;
  for (int64_t i{0}; i < group_size; i++) {
    uint8_t animation_period{static_cast<uint8_t>(stream.get())};
    uint8_t animation_duration{static_cast<uint8_t>(stream.get())};
    /* The vector containing animations is created and space is reserved for
     * containing all of them at the same time. Then the stream is read
     * directly into the vector. */
    auto animation{vector<uint16_t>(animation_period, 0)};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(animation.data()),
                animation_period * 2);  // Two bytes per sprite.
    animations_per_sprite_index.emplace(
        piecewise_construct, forward_as_tuple(animation[0]),
        forward_as_tuple(move(animation), animation_period,
                         animation_duration));
  }

  return animations_per_sprite_index;
}
