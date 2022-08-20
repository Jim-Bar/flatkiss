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

#ifndef SPRITESET_HPP_INCLUDED
#define SPRITESET_HPP_INCLUDED

#include <SDL2/SDL.h>

#include <string>
#include <vector>

/**
 * @brief Models a spriteset.
 *
 * A spriteset is a picture containing multiple sprites. It provides handy
 * methods to get information on the spriteset and to easily render sprites.
 * Charactersets and tilesets are in fact spritesets.
 *
 * This class also handles the lifecycle of the texture of the spriteset.
 */
class Spriteset {
 public:
  Spriteset(int64_t sprites_width, int64_t sprites_height,
            int64_t width_in_sprites, int64_t height_in_sprites,
            int64_t left_offset, int64_t top_offset, int64_t gap,
            int64_t texture_index, int64_t alpha_red, int64_t alpha_green,
            int64_t alpha_blue);
  Spriteset(Spriteset const& other) = delete;
  Spriteset(Spriteset&& other) = default;
  Spriteset& operator=(Spriteset const& other) = delete;
  Spriteset& operator=(Spriteset&& other) = delete;
  int64_t alpha_blue() const;
  int64_t alpha_green() const;
  int64_t alpha_red() const;
  int64_t gap() const;
  int64_t heightInSprites() const;
  int64_t leftOffset() const;
  SDL_Rect rectForSpriteIndex(int64_t sprite_index) const;
  int64_t spritesHeight() const;
  int64_t spritesWidth() const;
  int64_t textureIndex() const;
  int64_t topOffset() const;
  int64_t widthInSprites() const;

 private:
  int64_t const alpha_blue_;
  int64_t const alpha_green_;
  int64_t const alpha_red_;
  int64_t const gap_;
  int64_t const height_in_sprites_;
  int64_t const left_offset_;
  int64_t const sprites_height_;
  int64_t const sprites_width_;
  int64_t const texture_index_;
  int64_t const top_offset_;
  int64_t const width_in_sprites_;
};

/**
 * @brief Helper class for loading the spritesets from a file.
 */
class SpritesetLoader {
 public:
  static std::vector<Spriteset> load(std::string const& file_path);
};

#endif
