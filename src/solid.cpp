#include "solid.hpp"

#include <algorithm>
#include <fstream>
#include <limits>
#include <utility>
#include <vector>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::max;
using std::min;
using std::move;
using std::numeric_limits;
using std::piecewise_construct;
using std::string;
using std::unordered_map;
using std::vector;

Solid::Solid(vector<PositionedEllipse> positioned_ellipses,
             vector<PositionedRectangle> positioned_rectangles)
    : bounding_box_{Solid::computeBoundingBox(positioned_ellipses,
                                              positioned_rectangles)},
      positioned_ellipses_{move(positioned_ellipses)},
      positioned_rectangles_{move(positioned_rectangles)} {}

PositionedRectangle const& Solid::boundingBox() const { return bounding_box_; }

PositionedRectangle Solid::computeBoundingBox(
    std::vector<PositionedEllipse> positioned_ellipses,
    std::vector<PositionedRectangle> positioned_rectangles) {
  // This assumes there is at least one item.
  int64_t max_x{numeric_limits<int64_t>::min()};
  int64_t max_y{numeric_limits<int64_t>::min()};
  int64_t min_x{numeric_limits<int64_t>::max()};
  int64_t min_y{numeric_limits<int64_t>::max()};

  for (auto const& positioned_ellipse : positioned_ellipses) {
    max_x = max(max_x, positioned_ellipse.x() + positioned_ellipse.radiusX());
    max_y = max(max_y, positioned_ellipse.y() + positioned_ellipse.radiusY());
    min_x = min(min_x, positioned_ellipse.x() - positioned_ellipse.radiusX());
    min_y = min(min_y, positioned_ellipse.y() - positioned_ellipse.radiusY());
  }

  for (auto const& positioned_rectangle : positioned_rectangles) {
    max_x = max(max_x, positioned_rectangle.x() + positioned_rectangle.width());
    max_y =
        max(max_y, positioned_rectangle.y() + positioned_rectangle.height());
    min_x = min(min_x, positioned_rectangle.x());
    min_y = min(min_y, positioned_rectangle.y());
  }

  return PositionedRectangle{Position{min_x, min_y},
                             Rectangle{max_x - min_x, max_y - min_y}};
}

vector<PositionedEllipse> const& Solid::positionedEllipses() const {
  return positioned_ellipses_;
}

vector<PositionedRectangle> const& Solid::positionedRectangles() const {
  return positioned_rectangles_;
}

unordered_map<int64_t, Solid const> SolidLoader::load(string const& file_path) {
  unordered_map<int64_t, Solid const> solids_per_index;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      uint16_t solid_index{0};
      uint16_t solid_size{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&solid_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&solid_size), 2);
      solids_per_index.emplace(
          piecewise_construct, forward_as_tuple(solid_index),
          forward_as_tuple(move(loadSolid(solid_size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return solids_per_index;
}

Solid SolidLoader::loadSolid(int64_t solid_size, ifstream& solids_stream) {
  vector<PositionedRectangle> rectangles;
  vector<PositionedEllipse> ellipses;
  for (int64_t i{0}; i < solid_size; i++) {
    uint8_t collision_type{static_cast<uint8_t>(solids_stream.get())};
    Position position{static_cast<uint8_t>(solids_stream.get()),
                      static_cast<uint8_t>(solids_stream.get())};
    switch (collision_type) {
      case 0:  // The shape is a positioned rectangle.
        rectangles.emplace_back(
            position, Rectangle{static_cast<uint8_t>(solids_stream.get()),
                                static_cast<uint8_t>(solids_stream.get())});
        break;
      case 1:  // The shape is a positioned ellipse.
        ellipses.emplace_back(
            position, Ellipse{static_cast<uint8_t>(solids_stream.get()),
                              static_cast<uint8_t>(solids_stream.get())});
        break;
    }
  }

  return Solid{ellipses, rectangles};
}
