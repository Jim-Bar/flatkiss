#include "sprite_indices.hpp"

#include <fstream>
#include <utility>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::piecewise_construct;
using std::string;
using std::unordered_map;

SpriteIndices::SpriteIndices(
    unordered_map<Action, uint16_t>&& action_to_indices)
    : action_to_indices_{move(action_to_indices)} {}

uint16_t SpriteIndices::spriteIndexForAction(Action const& action) const {
  if (action_to_indices_.contains(action)) {
    return action_to_indices_.at(action);
  }

  return 0;  // FIXME: Raise exception.
}

Action SpriteIndicesLoader::actionIdentifierToAction(
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

unordered_map<int64_t, SpriteIndices const> SpriteIndicesLoader::load(
    string const& indices_file_path) {
  // FIXME: Find a better name for SpriteIndices (double plural here).
  unordered_map<int64_t, SpriteIndices const> sprite_indices_per_group;
  ifstream stream;
  stream.open(indices_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      uint16_t group_index{0};
      uint16_t group_size{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&group_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&group_size), 2);
      sprite_indices_per_group.emplace(
          piecewise_construct, forward_as_tuple(group_index),
          forward_as_tuple(move(loadGroup(group_size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return sprite_indices_per_group;
}

unordered_map<Action, uint16_t> SpriteIndicesLoader::loadGroup(
    int64_t group_size, ifstream& stream) {
  unordered_map<Action, uint16_t> action_to_indices;
  for (int64_t i{0}; i < group_size; i++) {
    uint16_t action_identifier{0};
    uint16_t sprite_index{0};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(&action_identifier), 2);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(&sprite_index), 2);
    action_to_indices[actionIdentifierToAction(action_identifier)] =
        sprite_index;
  }

  return action_to_indices;
}
