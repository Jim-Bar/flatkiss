#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <vector>

#include "characterset.hpp"
#include "move_direction.hpp"
#include "navigator.hpp"
#include "positioned_rectangle.hpp"

/**
 * @brief A character in the level.
 *
 * Can be seen as an instance of a character characterset.
 */
class Character {
 public:
  Character(Characterset const& characterset, Navigator const& navigator,
            Position const& initialPosition, Rectangle const& rectangle);
  int64_t animationTick() const;
  Characterset const& characterset() const;
  int64_t height() const;
  void moveBy(Vector const& desired_displacement);
  MoveDirection const& movingDirection() const;
  Position const& position() const;
  Rectangle const& rectangle() const;
  int64_t width() const;
  int64_t x() const;
  int64_t y() const;

 private:
  int64_t animation_tick_{0};
  Characterset const& characterset_;
  MoveDirection moving_direction_;
  Navigator const& navigator_;
  PositionedRectangle positioned_rectangle_;

  void resetAnimationTick();
  void updateMovingDirection(Vector const& desired_displacement,
                             Vector const& actual_displacement);
  void updateMovingDirectionForDisplacement(Vector const& displacement);
};

/**
 * @brief Helper class for loading the characters from a file.
 */
class CharacterLoader {
 public:
  static std::vector<Character> load(
      std::string const& characters_file_path,
      std::vector<Characterset> const& charactersets,
      Navigator const& navigator, int64_t tiles_size);

 private:
  static int64_t constexpr kAnimationFieldSize{2};
  static int64_t constexpr kCharactersetFieldSize{2};
  static int64_t constexpr kControllerFieldSize{1};
  static int64_t constexpr kCollisionFieldSize{2};
  static int64_t constexpr kXFieldSize{8};
  static int64_t constexpr kYFieldSize{8};
};

#endif
