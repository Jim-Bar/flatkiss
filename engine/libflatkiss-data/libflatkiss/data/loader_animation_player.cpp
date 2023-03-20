/*
 * Copyright (C) 2021-2023 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
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
#include <libflatkiss/data/stream_reader.hpp>
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
      int64_t group_index{StreamReader::read(stream, 2)};
      int64_t group_size{StreamReader::read(stream, 2)};
      animation_players_per_group.emplace(
          piecewise_construct, forward_as_tuple(group_index),
          forward_as_tuple(move(loadGroup(group_size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return animation_players_per_group;
}

unordered_map<int64_t, Animation> LoaderAnimationPlayer::loadGroup(
    int64_t group_size, ifstream& stream) {
  unordered_map<int64_t, Animation> animations_per_sprite_index;
  for (int64_t i{0}; i < group_size; i++) {
    int64_t period{StreamReader::read(stream, 1)};
    int64_t duration{StreamReader::read(stream, 1)};
    /* The vector containing animations is created and space is reserved for
     * containing all of them at the same time. Then the stream is read
     * directly into the vector. */
    vector<int64_t> sprites(period, 0);
    for (int64_t i{0}; i < period; i++) {
      sprites[i] = StreamReader::read(stream, 2);
    }
    animations_per_sprite_index.emplace(
        piecewise_construct, forward_as_tuple(sprites[0]),
        forward_as_tuple(move(sprites), period, duration));
  }

  return animations_per_sprite_index;
}
