#include "collision.hpp"

#include <fstream>
#include <set>
#include <vector>

using std::move;
using std::set;
using std::unordered_map;
using std::vector;

Collision::Collision(std::vector<PositionedEllipse> const positioned_ellipses,
                     vector<PositionedRectangle> const positioned_rectangles)
    : positioned_ellipses_(move(positioned_ellipses)),
      positioned_rectangles_(move(positioned_rectangles)) {}

bool Collision::collidesWith(PositionedRectangle const& positioned_rectangle,
                             Position const when_at_position) const {
  for (auto ellipse : positioned_ellipses_) {
    if (positioned_rectangle.intersectsWith(when_at_position + ellipse)) {
      return true;
    }
  }

  for (auto rectangle : positioned_rectangles_) {
    if (positioned_rectangle.intersectsWith(when_at_position + rectangle)) {
      return true;
    }
  }

  return false;
}

unordered_map<uint16_t, Collision const> CollisionLoader::load(
    std::string const& file_path) {
  // First read all the positioned rectangles for all the tile indices.
  std::ifstream stream;
  set<uint16_t> all_tiles_indices;
  unordered_map<uint16_t, vector<PositionedEllipse>> ellipses_per_tile_index;
  unordered_map<uint16_t, vector<PositionedRectangle>>
      rectangles_per_tile_index;
  stream.open(file_path, std::ios::in | std::ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != std::istream::traits_type::eof()) {
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
        std::piecewise_construct, std::forward_as_tuple(tile_index),
        std::forward_as_tuple(move(ellipses_per_tile_index[tile_index]),
                              move(rectangles_per_tile_index[tile_index])));
  }

  return collisions_per_tile_index;
}
