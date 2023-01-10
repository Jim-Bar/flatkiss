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
using std::unordered_map;
using std::vector;

unordered_map<int64_t, Solid const> LoaderSolid::load(string const& file_path) {
  unordered_map<int64_t, Solid const> solids_per_index;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t solid_index{StreamReader::read(stream, 2)};
      int64_t solid_size{StreamReader::read(stream, 2)};
      solids_per_index.emplace(
          piecewise_construct, forward_as_tuple(solid_index),
          forward_as_tuple(move(loadSolid(solid_size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return solids_per_index;
}

Solid LoaderSolid::loadSolid(int64_t solid_size, ifstream& solids_stream) {
  vector<PositionedRectangle> rectangles;
  vector<PositionedEllipse> ellipses;
  for (int64_t i{0}; i < solid_size; i++) {
    int64_t collision_type{StreamReader::read(solids_stream, 1)};
    int64_t x{StreamReader::read(solids_stream, 1)};
    int64_t y{StreamReader::read(solids_stream, 1)};
    int64_t width{StreamReader::read(solids_stream, 1)};
    int64_t height{StreamReader::read(solids_stream, 1)};
    Position position{x, y};
    switch (collision_type) {
      case 0:  // The shape is a positioned rectangle.
        rectangles.emplace_back(position, Rectangle{width, height});
        break;
      case 1:  // The shape is a positioned ellipse.
        ellipses.emplace_back(position, Ellipse{width, height});
        break;
    }
  }

  return Solid{ellipses, rectangles};
}
