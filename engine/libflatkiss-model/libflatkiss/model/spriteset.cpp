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

#include <libflatkiss/model/spriteset.hpp>

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
