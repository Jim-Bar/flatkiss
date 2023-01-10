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
#include <libflatkiss/data/loader_spriteset.hpp>
#include <libflatkiss/data/stream_reader.hpp>

using std::ifstream;
using std::ios;
using std::istream;
using std::string;
using std::vector;

vector<Spriteset> LoaderSpriteset::load(string const& file_path) {
  vector<Spriteset> spritesets;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t sprites_width{StreamReader::read(stream, 1)};
      int64_t sprites_height{StreamReader::read(stream, 1)};
      int64_t width_in_sprites{StreamReader::read(stream, 2)};
      int64_t height_in_tiles{StreamReader::read(stream, 2)};
      int64_t top_offset{StreamReader::read(stream, 2)};
      int64_t left_offset{StreamReader::read(stream, 2)};
      int64_t gap{StreamReader::read(stream, 2)};
      int64_t texture_index{StreamReader::read(stream, 2)};
      int64_t alpha_red{StreamReader::read(stream, 1)};
      int64_t alpha_green{StreamReader::read(stream, 1)};
      int64_t alpha_blue{StreamReader::read(stream, 1)};
      spritesets.emplace_back(sprites_width, sprites_height, width_in_sprites,
                              height_in_tiles, left_offset, top_offset, gap,
                              texture_index, alpha_red, alpha_green,
                              alpha_blue);
    }
    stream.close();
  }  // FIXME: Raise exception.

  return spritesets;
}
