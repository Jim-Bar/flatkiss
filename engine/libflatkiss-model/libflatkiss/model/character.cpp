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

#include <libflatkiss/model/character.hpp>
#include <set>
#include <utility>

using std::move;
using std::set;
using std::vector;

Character::Character(Spriteset const& spriteset,
                     ActionSpriteMapper const& action_sprite_mapper,
                     AnimationPlayer const& animation_player,
                     vector<ControllerType> const& controllers,
                     Solid const& solid, Position const& initial_position)
    : spriteset_{spriteset},
      action_sprite_mapper_{action_sprite_mapper},
      animation_player_{animation_player},
      controllers_{controllers},
      facing_direction_{CardinalDirection::kSouth},
      positioned_solid_{initial_position, solid} {}

vector<ControllerType> const& Character::controllers() const {
  return controllers_;
}

Action Character::currentAction() const {
  switch (facing_direction_) {
    case kWest:
      return Action::kWalkLeft;
    case kSouth:
      return Action::kWalkDown;
    case kEast:
      return Action::kWalkRight;
    case kNorth:
      return Action::kWalkUp;
    default:
      return Action::kWalkLeft;  // FIXME.
  }
}

void Character::moveTo(Position&& new_position) {
  positioned_solid_.position(move(new_position));
}

Position const& Character::position() const {
  return positioned_solid_.position();
}

PositionedSolid const& Character::positionedSolid() const {
  return positioned_solid_;
}

void Character::resetAnimationTick() {
  /* Not resetting to zero. Instead resetting to the last tick before which the
   * next animation is played. Consequently, next time the character moves, it
   * immediately starts animating. In particular, this prevents it from sliding
   * for small moves. If this causes issues in the future, it could be replaced
   * by adding pre-animation, played a single time when the animation starts. */
  animation_tick_ =
      animation_player_.animationDurationForSpriteIndex(spriteIndex()) - 1;
}

uint16_t Character::spriteIndex() const {
  return animation_player_.animatedSpriteIndexFor(
      action_sprite_mapper_.spriteIndexForAction(currentAction()),
      animation_tick_);
}

Spriteset const& Character::spriteset() const { return spriteset_; }

void Character::updateFacingDirection(Vector const& desired_displacement,
                                      Vector const& actual_displacement) {
  /* Using the desired displacement as fallback ensures that when the character
   * is blocked, it still turns toward the tried direction. */
  if (actual_displacement != Vector::kZero) {
    animation_tick_++;
    updateFacingDirectionForDisplacement(actual_displacement);
  } else {
    resetAnimationTick();
    updateFacingDirectionForDisplacement(desired_displacement);
  }
}

void Character::updateFacingDirectionForDisplacement(
    Vector const& displacement) {
  set<CardinalDirection> facing_directions;
  if (displacement.dx() < 0) {
    facing_directions.insert(CardinalDirection::kWest);
  } else if (displacement.dx() > 0) {
    facing_directions.insert(CardinalDirection::kEast);
  }

  if (displacement.dy() < 0) {
    facing_directions.insert(CardinalDirection::kNorth);
  } else if (displacement.dy() > 0) {
    facing_directions.insert(CardinalDirection::kSouth);
  }

  /* Favour the current moving direction if it is still active. Otherwise pick a
   * random direction. Because direction changes are sequential, picking a
   * "random" direction is always right. */
  if (!facing_directions.empty() &&
      !facing_directions.contains(facing_direction_)) {
    // The direction changed.
    facing_direction_ = *facing_directions.begin();
    resetAnimationTick();
  }
}

int64_t Character::x() const { return position().x(); }

int64_t Character::y() const { return position().y(); }
