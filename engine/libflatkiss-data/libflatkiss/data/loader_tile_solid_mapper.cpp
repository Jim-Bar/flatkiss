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
#include <libflatkiss/data/loader_tile_solid_mapper.hpp>
#include <libflatkiss/data/stream_reader.hpp>
#include <utility>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::piecewise_construct;
using std::string;
using std::unordered_map;

unordered_map<int64_t, TileSolidMapper const> LoaderTileSolidMapper::load(
    string const& tile_solid_map_file_path) {
  unordered_map<int64_t, TileSolidMapper const> index_to_mapper;
  ifstream stream;
  stream.open(tile_solid_map_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t group_index{StreamReader::read(stream, 2)};
      int64_t group_size{StreamReader::read(stream, 2)};
      index_to_mapper.emplace(
          piecewise_construct, forward_as_tuple(group_index),
          forward_as_tuple(move(loadGroup(group_size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return index_to_mapper;
}

unordered_map<Sprite, int64_t> LoaderTileSolidMapper::loadGroup(
    int64_t group_size, std::ifstream& stream) {
  unordered_map<Sprite, int64_t> tiles_to_solids;
  for (int64_t i{0}; i < group_size; i++) {
    int64_t tile_index{StreamReader::read(stream, 2)};
    int64_t solid_index{StreamReader::read(stream, 2)};
    tiles_to_solids[tile_index] = solid_index;
  }

  return tiles_to_solids;
}
