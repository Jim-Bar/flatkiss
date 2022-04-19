#include "collider.hpp"

#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::piecewise_construct;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

Collider::Collider(
    unordered_map<uint16_t, Collision>&& collisions_per_tile_index)
    : collisions_per_tile_index_{move(collisions_per_tile_index)} {}

bool Collider::collide(PositionedRectangle const& positioned_rectangle,
                       uint16_t tile_index,
                       Position const& tile_position) const {
  if (!collisions_per_tile_index_.contains(tile_index)) {
    return false;
  }

  Collision const& collision{collisions_per_tile_index_.at(tile_index)};

  return collision.collidesWith(positioned_rectangle, tile_position);
}

Collision const& Collider::zero() const {
  return collisions_per_tile_index_.at(0);
}

unordered_map<int64_t, Collider const> ColliderLoader::load(
    string const& file_path) {
  unordered_map<int64_t, Collider const> colliders_per_group;
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
      colliders_per_group.emplace(
          piecewise_construct, forward_as_tuple(group_index),
          forward_as_tuple(move(loadGroup(group_size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return colliders_per_group;
}

unordered_map<uint16_t, Collision> ColliderLoader::loadGroup(
    int64_t group_size, ifstream& collisions_stream) {
  // First read all the positioned rectangles for all the tile indices.
  set<uint16_t> all_tiles_indices;
  unordered_map<uint16_t, vector<PositionedEllipse>> ellipses_per_tile_index;
  unordered_map<uint16_t, vector<PositionedRectangle>>
      rectangles_per_tile_index;
  for (int64_t i{0}; i < group_size; i++) {
    uint16_t tile_index{0};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    collisions_stream.read(reinterpret_cast<char*>(&tile_index),
                           2);  // Two bytes per tile index.
    all_tiles_indices.insert(tile_index);
    uint8_t collision_type{static_cast<uint8_t>(collisions_stream.get())};
    Position position{static_cast<uint8_t>(collisions_stream.get()),
                      static_cast<uint8_t>(collisions_stream.get())};
    switch (collision_type) {
      case 0:  // The collision is a positioned rectangle.
        if (!rectangles_per_tile_index.contains(tile_index)) {
          rectangles_per_tile_index.emplace(tile_index,
                                            vector<PositionedRectangle>{});
        }
        rectangles_per_tile_index[tile_index].emplace_back(
            position, Rectangle{static_cast<uint8_t>(collisions_stream.get()),
                                static_cast<uint8_t>(collisions_stream.get())});
        break;
      case 1:  // The collision is a positioned ellipse.
        if (!ellipses_per_tile_index.contains(tile_index)) {
          ellipses_per_tile_index.emplace(tile_index,
                                          vector<PositionedEllipse>{});
        }
        ellipses_per_tile_index[tile_index].emplace_back(
            position, Ellipse{static_cast<uint8_t>(collisions_stream.get()),
                              static_cast<uint8_t>(collisions_stream.get())});
        break;
    }
  }

  /* Secondly, once all the positioned rectangles / ellipses are known, the
   * collisions can be built. */
  unordered_map<uint16_t, Collision> collisions_per_tile_index;
  for (auto tile_index : all_tiles_indices) {
    collisions_per_tile_index.emplace(
        piecewise_construct, forward_as_tuple(tile_index),
        forward_as_tuple(move(ellipses_per_tile_index[tile_index]),
                         move(rectangles_per_tile_index[tile_index])));
  }

  return collisions_per_tile_index;
}
