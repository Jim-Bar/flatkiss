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
#include <libflatkiss/data/loader_solid.hpp>
#include <utility>
#include <vector>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::piecewise_construct;
using std::string;
using std::unordered_map;
using std::vector;

struct DataSolidInfo {
  uint16_t index;
  uint16_t size;
};

struct DataSolid {
  uint8_t collision_type;
  uint8_t x;
  uint8_t y;
  uint8_t width;
  uint8_t height;
};

unordered_map<int64_t, Solid const> LoaderSolid::load(string const& file_path) {
  unordered_map<int64_t, Solid const> solids_per_index;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      DataSolidInfo solid_info{};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&solid_info), sizeof(DataSolidInfo));
      solids_per_index.emplace(
          piecewise_construct, forward_as_tuple(solid_info.index),
          forward_as_tuple(move(loadSolid(solid_info.size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return solids_per_index;
}

Solid LoaderSolid::loadSolid(int64_t solid_size, ifstream& solids_stream) {
  vector<PositionedRectangle> rectangles;
  vector<PositionedEllipse> ellipses;
  for (int64_t i{0}; i < solid_size; i++) {
    DataSolid solid{};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    solids_stream.read(reinterpret_cast<char*>(&solid), sizeof(DataSolid));
    uint8_t collision_type{solid.collision_type};
    Position position{solid.x, solid.y};
    switch (collision_type) {
      case 0:  // The shape is a positioned rectangle.
        rectangles.emplace_back(position, Rectangle{solid.width, solid.height});
        break;
      case 1:  // The shape is a positioned ellipse.
        ellipses.emplace_back(position, Ellipse{solid.width, solid.height});
        break;
    }
  }

  return Solid{ellipses, rectangles};
}
