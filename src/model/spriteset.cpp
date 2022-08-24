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

#include "spriteset.hpp"

#include <fstream>
#include <utility>

using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::string;
using std::to_string;
using std::tuple;
using std::vector;

Spriteset::Spriteset(int64_t sprites_width, int64_t sprites_height,
                     int64_t width_in_sprites, int64_t height_in_sprites,
                     int64_t left_offset, int64_t top_offset, int64_t gap,
                     int64_t texture_index, int64_t alpha_red,
                     int64_t alpha_green, int64_t alpha_blue)
    : sprites_width_{sprites_width},
      sprites_height_{sprites_height},
      width_in_sprites_{width_in_sprites},
      height_in_sprites_{height_in_sprites},
      left_offset_{left_offset},
      top_offset_{top_offset},
      gap_{gap},
      texture_index_{texture_index},
      alpha_red_{alpha_red},
      alpha_green_{alpha_green},
      alpha_blue_{alpha_blue} {}

int64_t Spriteset::alpha_blue() const { return alpha_blue_; }

int64_t Spriteset::alpha_green() const { return alpha_green_; }

int64_t Spriteset::alpha_red() const { return alpha_red_; }

int64_t Spriteset::gap() const { return gap_; }

int64_t Spriteset::heightInSprites() const { return height_in_sprites_; }

int64_t Spriteset::leftOffset() const { return left_offset_; }

int64_t Spriteset::spritesHeight() const { return sprites_height_; }

int64_t Spriteset::spritesWidth() const { return sprites_width_; }

int64_t Spriteset::textureIndex() const { return texture_index_; }

int64_t Spriteset::topOffset() const { return top_offset_; }

int64_t Spriteset::widthInSprites() const { return width_in_sprites_; }

vector<Spriteset> SpritesetLoader::load(string const& file_path) {
  vector<Spriteset> spritesets;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      // FIXME: Use stream.get() for uint8_t variables.
      uint8_t sprites_width{0};
      uint8_t sprites_height{0};
      uint16_t width_in_sprites{0};
      uint16_t height_in_tiles{0};
      uint16_t top_offset{0};
      uint16_t left_offset{0};
      uint16_t gap{0};
      uint16_t texture_index{0};
      uint8_t alpha_red{0};
      uint8_t alpha_green{0};
      uint8_t alpha_blue{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&sprites_width), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&sprites_height), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&width_in_sprites), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&height_in_tiles), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&top_offset), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&left_offset), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&gap), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&texture_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&alpha_red), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&alpha_green), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&alpha_blue), 1);
      spritesets.emplace_back(sprites_width, sprites_height,
                                 width_in_sprites, height_in_tiles, left_offset,
                                 top_offset, gap, texture_index, alpha_red,
                                 alpha_green, alpha_blue);
    }
    stream.close();
  }  // FIXME: Raise exception.

  return spritesets;
}
