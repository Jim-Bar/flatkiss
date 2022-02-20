#include "collision.hpp"

#include <algorithm>
#include <fstream>
#include <set>
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
using std::ranges::any_of;

Collision::Collision(vector<PositionedEllipse> positioned_ellipses,
                     vector<PositionedRectangle> positioned_rectangles)
    : positioned_ellipses_(move(positioned_ellipses)),
      positioned_rectangles_(move(positioned_rectangles)) {}

bool Collision::collidesWith(PositionedRectangle const& positioned_rectangle,
                             Position const& when_at_position) const {
  for (auto ellipse : positioned_ellipses_) {
    if (positioned_rectangle.intersectsWith(when_at_position + ellipse)) {
      return true;
    }
  }

  return any_of(positioned_rectangles_,
                [&](PositionedRectangle const& rectangle) {
                  return positioned_rectangle.intersectsWith(when_at_position +
                                                             rectangle);
                });
}

unordered_map<uint16_t, Collision const> CollisionLoader::load(
    string const& file_path) {
  // First read all the positioned rectangles for all the tile indices.
  ifstream stream;
  set<uint16_t> all_tiles_indices;
  unordered_map<uint16_t, vector<PositionedEllipse>> ellipses_per_tile_index;
  unordered_map<uint16_t, vector<PositionedRectangle>>
      rectangles_per_tile_index;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      uint16_t tile_index{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&tile_index),
                  2);  // Two bytes per tile index.
      all_tiles_indices.insert(tile_index);
      uint8_t collision_type{static_cast<uint8_t>(stream.get())};
      Position position{static_cast<uint8_t>(stream.get()),
                        static_cast<uint8_t>(stream.get())};
      switch (collision_type) {
        case 0:  // The collision is a positioned rectangle.
          if (!rectangles_per_tile_index.contains(tile_index)) {
            rectangles_per_tile_index.emplace(tile_index,
                                              vector<PositionedRectangle>{});
          }
          rectangles_per_tile_index[tile_index].emplace_back(
              position, Rectangle{static_cast<uint8_t>(stream.get()),
                                  static_cast<uint8_t>(stream.get())});
          break;
        case 1:  // The collision is a positioned ellipse.
          if (!ellipses_per_tile_index.contains(tile_index)) {
            ellipses_per_tile_index.emplace(tile_index,
                                            vector<PositionedEllipse>{});
          }
          ellipses_per_tile_index[tile_index].emplace_back(
              position, Ellipse{static_cast<uint8_t>(stream.get()),
                                static_cast<uint8_t>(stream.get())});
          break;
      }
    }
    stream.close();
  }  // FIXME: Raise exception.

  /* Secondly, once all the positioned rectangles / ellipses are known, the
   * collisions can be built. */
  unordered_map<uint16_t, Collision const> collisions_per_tile_index;
  for (auto tile_index : all_tiles_indices) {
    collisions_per_tile_index.emplace(
        piecewise_construct, forward_as_tuple(tile_index),
        forward_as_tuple(move(ellipses_per_tile_index[tile_index]),
                         move(rectangles_per_tile_index[tile_index])));
  }

  return collisions_per_tile_index;
}
