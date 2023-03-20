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

#ifndef LIBFLATKISS_MODEL_CHARACTER_HPP_INCLUDED
#define LIBFLATKISS_MODEL_CHARACTER_HPP_INCLUDED

#include <libflatkiss/model/action_sprite_mapper.hpp>
#include <libflatkiss/model/animation_player.hpp>
#include <libflatkiss/model/cardinal_direction.hpp>
#include <libflatkiss/model/character_template.hpp>
#include <libflatkiss/model/controller_type.hpp>
#include <libflatkiss/model/positioned_solid.hpp>
#include <libflatkiss/model/rectangle.hpp>
#include <libflatkiss/model/spriteset.hpp>
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
            AnimationPlayer const& animation_player,
            std::vector<ControllerType> const& controllers, Solid const& solid,
            Position const& initial_position);
  std::vector<ControllerType> const& controllers() const;
  void moveTo(Position&& new_position);
  Position const& position() const;
  PositionedSolid const& positionedSolid() const;
  Sprite const& sprite() const;
  Spriteset const& spriteset() const;
  void updateFacingDirection(Vector const& desired_displacement,
                             Vector const& actual_displacement);
  int64_t x() const;
  int64_t y() const;

 private:
  AnimationPlayer const& animation_player_;
  int64_t animation_tick_{0};
  std::vector<ControllerType> const controllers_;
  Spriteset const& spriteset_;
  CardinalDirection facing_direction_;
  PositionedSolid positioned_solid_;
  ActionSpriteMapper const& action_sprite_mapper_;

  Action currentAction() const;
  void resetAnimationTick();
  void updateFacingDirectionForDisplacement(Vector const& displacement);
};

#endif
