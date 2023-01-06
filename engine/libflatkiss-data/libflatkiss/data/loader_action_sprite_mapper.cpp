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
#include <libflatkiss/data/loader_action_sprite_mapper.hpp>
#include <utility>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::piecewise_construct;
using std::string;
using std::unordered_map;

struct DataGroupInfo {
  uint16_t index;
  uint16_t size;
};

struct DataGroup {
  uint16_t action_identifier;
  uint16_t sprite_index;
};

Action LoaderActionSpriteMapper::actionIdentifierToAction(
    uint16_t action_identifier) {
  switch (action_identifier) {
    case 0:
      return Action::kWalkLeft;
    case 1:
      return Action::kWalkDown;
    case 2:
      return Action::kWalkRight;
    case 3:
      return Action::kWalkUp;
    default:
      return Action::kWalkLeft;  // FIXME: Raise exception;
  }
}

unordered_map<int64_t, ActionSpriteMapper const> LoaderActionSpriteMapper::load(
    string const& indices_file_path) {
  unordered_map<int64_t, ActionSpriteMapper const> index_to_mapper;
  ifstream stream;
  stream.open(indices_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      DataGroupInfo group_info{};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&group_info), sizeof(DataGroupInfo));
      index_to_mapper.emplace(
          piecewise_construct, forward_as_tuple(group_info.index),
          forward_as_tuple(move(loadGroup(group_info.size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return index_to_mapper;
}

unordered_map<Action, uint16_t> LoaderActionSpriteMapper::loadGroup(
    int64_t group_size, ifstream& stream) {
  unordered_map<Action, uint16_t> action_to_indices;
  for (int64_t i{0}; i < group_size; i++) {
    DataGroup group{};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(&group), sizeof(DataGroup));
    action_to_indices[actionIdentifierToAction(group.action_identifier)] =
        group.sprite_index;
  }

  return action_to_indices;
}
