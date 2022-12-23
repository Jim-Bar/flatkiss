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

#ifndef LIBFLATKISS_MODEL_CHARACTER_HPP_INCLUDED
#define LIBFLATKISS_MODEL_CHARACTER_HPP_INCLUDED

#include <libflatkiss/model/action_sprite_mapper.hpp>
#include <libflatkiss/model/animation_player.hpp>
#include <libflatkiss/model/cardinal_direction.hpp>
#include <libflatkiss/model/character_template.hpp>
#include <libflatkiss/model/navigator.hpp>
#include <libflatkiss/model/positioned_solid.hpp>
#include <libflatkiss/model/rectangle.hpp>
#include <libflatkiss/model/spriteset.hpp>
#include <tuple>
#include <unordered_map>
#include <vector>

// Forward declaration to break the cycle Character / Level.
class Level;

/**
 * @brief A character in the level.
 *
 * Can be seen as an instance of a character spriteset.
 */
class Character {
 public:
  Character(Spriteset const& spriteset,
            ActionSpriteMapper const& action_sprite_mapper,
            AnimationPlayer const& animation_player, Solid const& solid,
            Level const& level, Navigator const& navigator,
            Position const& initial_position);
  void moveBy(Vector const& desired_displacement);
  Position const& position() const;
  uint16_t spriteIndex() const;
  Spriteset const& spriteset() const;
  int64_t x() const;
  int64_t y() const;

 private:
  AnimationPlayer const& animation_player_;
  int64_t animation_tick_{0};
  Spriteset const& spriteset_;
  CardinalDirection facing_direction_;
  Level const& level_;
  Navigator const& navigator_;
  PositionedSolid positioned_solid_;
  ActionSpriteMapper const& action_sprite_mapper_;

  Action currentAction() const;
  void resetAnimationTick();
  void updateFacingDirection(Vector const& desired_displacement,
                             Vector const& actual_displacement);
  void updateFacingDirectionForDisplacement(Vector const& displacement);
};

/**
 * @brief Helper class for loading the characters from a file.
 */
class CharacterLoader {
 public:
  static std::tuple<std::vector<int64_t>, std::vector<CharacterTemplate>> load(
      std::string const& characters_file_path,
      std::vector<Spriteset> const& spritesets,
      std::unordered_map<int64_t, ActionSpriteMapper const> const&
          action_sprite_mappers,
      std::unordered_map<int64_t, AnimationPlayer const> const&
          animation_players,
      std::unordered_map<int64_t, Solid const> const& solids,
      Navigator const& navigator, int64_t tiles_width, int64_t tiles_height);

 private:
  static int64_t constexpr kAnimationFieldSize{2};
  static int64_t constexpr kControllerFieldSize{1};
  static int64_t constexpr kCollisionFieldSize{2};
  static int64_t constexpr kIndicesFieldSize{2};
  static int64_t constexpr kSpritesetFieldSize{2};
  static int64_t constexpr kXFieldSize{8};
  static int64_t constexpr kYFieldSize{8};
};

#endif
