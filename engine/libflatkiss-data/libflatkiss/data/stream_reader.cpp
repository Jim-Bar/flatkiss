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

#include <libflatkiss/data/stream_reader.hpp>
#include <vector>

using std::istream;
using std::vector;

int64_t StreamReader::read(istream& stream, int64_t num_bytes) {
  constexpr int64_t kNumBitsInAByte{8};
  int64_t value{0};
  vector<uint8_t> buf(num_bytes, 0);
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  stream.read(reinterpret_cast<char*>(buf.data()), num_bytes);
  for (int64_t i{0}; i < num_bytes; i++) {
    value |= static_cast<int64_t>(buf[i]) << i * kNumBitsInAByte;
  }

  return value;
}
