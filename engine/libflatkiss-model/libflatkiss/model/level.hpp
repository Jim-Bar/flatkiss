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

#ifndef LIBFLATKISS_MODEL_LEVEL_HPP_INCLUDED
#define LIBFLATKISS_MODEL_LEVEL_HPP_INCLUDED

#include <libflatkiss/model/animation_player.hpp>
#include <libflatkiss/model/character.hpp>
#include <libflatkiss/model/character_template.hpp>
#include <libflatkiss/model/spriteset.hpp>
#include <libflatkiss/model/tile_solid_mapper.hpp>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Models a level.
 *
 * A level is a list of tiles indices, with dimensions. Once created, this class
 * provides handy methods to access the content of the level.
 */
class Level {
 public:
  Level(std::vector<uint16_t>&& tiles, int64_t width_in_tiles,
        int64_t height_in_tiles, Spriteset const& spriteset,
        AnimationPlayer const& animation_player,
        TileSolidMapper const& tile_solid_mapper);
  AnimationPlayer const& animationPlayer() const;
  std::vector<Character>& characters();
  int64_t heightInTiles() const;
  Spriteset const& spriteset() const;
  uint16_t tileIndex(int64_t i, int64_t j) const;
  TileSolidMapper const& tileSolidMapper() const;
  int64_t widthInTiles() const;

 private:
  AnimationPlayer const& animation_player_;
  std::vector<Character> characters_;
  int64_t const height_in_tiles_;
  Spriteset const& spriteset_;
  TileSolidMapper const& tile_solid_mapper_;
  std::vector<uint16_t> const tiles_;
  int64_t const width_in_tiles_;
};

/**
 * @brief Helper class for loading the level from a file.
 */
class LevelLoader {
 public:
  static std::vector<Level> load(
      std::string const& file_path, std::vector<Spriteset> const& spritesets,
      std::unordered_map<int64_t, AnimationPlayer const>& animation_players,
      std::unordered_map<int64_t, TileSolidMapper const>& tile_solid_mappers,
      std::vector<CharacterTemplate> const& character_templates,
      std::vector<Character> characters);
};

#endif
