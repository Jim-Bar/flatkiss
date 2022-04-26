#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <unordered_map>
#include <vector>

#include "action_sprite_mapper.hpp"
#include "animation_player.hpp"
#include "cardinal_direction.hpp"
#include "navigator.hpp"
#include "positioned_solid.hpp"
#include "rectangle.hpp"
#include "spriteset.hpp"

/**
 * @brief A character in the level.
 *
 * Can be seen as an instance of a character characterset.
 */
class Character {
 public:
  Character(Spriteset const& characterset,
            ActionSpriteMapper const& action_sprite_mapper,
            AnimationPlayer const& animation_player, Solid const& solid,
            Navigator const& navigator, Position const& initialPosition,
            Rectangle const& rectangle);
  Spriteset const& characterset() const;
  int64_t height() const;
  void moveBy(Vector const& desired_displacement);
  Position const& position() const;
  uint16_t spriteIndex() const;
  int64_t width() const;
  int64_t x() const;
  int64_t y() const;

 private:
  AnimationPlayer const& animation_player_;
  int64_t animation_tick_{0};
  Spriteset const& characterset_;
  CardinalDirection facing_direction_;
  Navigator const& navigator_;
  PositionedSolid positioned_solid_;
  Rectangle const rectangle_;
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
  static std::vector<Character> load(
      std::string const& characters_file_path,
      std::vector<Spriteset> const& charactersets,
      std::unordered_map<int64_t, ActionSpriteMapper const> const&
          action_sprite_mappers,
      std::unordered_map<int64_t, AnimationPlayer const> const&
          animation_players,
      std::unordered_map<int64_t, Solid const> const& solids,
      Navigator const& navigator, int64_t tiles_width, int64_t tiles_height);

 private:
  static int64_t constexpr kAnimationFieldSize{2};
  static int64_t constexpr kCharactersetFieldSize{2};
  static int64_t constexpr kControllerFieldSize{1};
  static int64_t constexpr kCollisionFieldSize{2};
  static int64_t constexpr kIndicesFieldSize{2};
  static int64_t constexpr kXFieldSize{8};
  static int64_t constexpr kYFieldSize{8};
};

#endif
