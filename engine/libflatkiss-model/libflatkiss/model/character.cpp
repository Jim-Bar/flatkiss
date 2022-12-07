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

#include <fstream>
#include <libflatkiss/model/character.hpp>
#include <set>
#include <utility>

using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::set;
using std::string;
using std::tuple;
using std::unordered_map;
using std::vector;

Character::Character(Spriteset const& spriteset,
                     ActionSpriteMapper const& action_sprite_mapper,
                     AnimationPlayer const& animation_player,
                     Solid const& solid, Navigator const& navigator,
                     Position const& initial_position)
    : spriteset_{spriteset},
      action_sprite_mapper_{action_sprite_mapper},
      animation_player_{animation_player},
      facing_direction_{CardinalDirection::kSouth},
      navigator_{navigator},
      positioned_solid_{initial_position, solid} {}

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

void Character::moveBy(Vector const& desired_displacement) {
  Position final_position{
      navigator_.moveBy(positioned_solid_, desired_displacement)};
  updateFacingDirection(desired_displacement, final_position - position());
  positioned_solid_.position(move(final_position));
}

Position const& Character::position() const {
  return positioned_solid_.position();
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

tuple<vector<int64_t>, vector<Character>> CharacterLoader::load(
    string const& characters_file_path, vector<Spriteset> const& spritesets,
    unordered_map<int64_t, ActionSpriteMapper const> const&
        action_sprite_mappers,
    unordered_map<int64_t, AnimationPlayer const> const& animation_players,
    unordered_map<int64_t, Solid const> const& solids,
    Navigator const& navigator, int64_t tiles_width, int64_t tiles_height) {
  vector<int64_t> characters_to_controllers;
  vector<Character> characters;
  ifstream stream;
  stream.open(characters_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t x{0};
      int64_t y{0};
      uint16_t spriteset_index{0};
      uint16_t action_sprite_mapper_index{0};
      uint16_t animations_index{0};
      uint16_t solid_index{0};
      uint8_t controller_type{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&x), kXFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&y), kYFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&spriteset_index),
                  kSpritesetFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&action_sprite_mapper_index),
                  kIndicesFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&animations_index),
                  kAnimationFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&solid_index), kCollisionFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&controller_type),
                  kControllerFieldSize);
      characters.emplace_back(
          spritesets[spriteset_index],
          action_sprite_mappers.at(action_sprite_mapper_index),
          animation_players.at(animations_index), solids.at(solid_index),
          navigator, Position{x * tiles_width, y * tiles_height});
      characters_to_controllers.emplace_back(controller_type);
    }
    stream.close();
  }  // FIXME: Raise exception.

  return {characters_to_controllers, characters};
}