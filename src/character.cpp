#include "character.hpp"

#include <fstream>
#include <set>
#include <utility>

using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

Character::Character(Spriteset const& characterset,
                     ActionSpriteMapper const& action_sprite_mapper,
                     AnimationPlayer const& animation_player,
                     Solid const& solid, Navigator const& navigator,
                     Position const& initialPosition,
                     Rectangle const& rectangle)
    : characterset_{characterset},
      action_sprite_mapper_{action_sprite_mapper},
      animation_player_{animation_player},
      moving_direction_{MovingDirection::kDown},
      navigator_{navigator},
      positioned_solid_{initialPosition, solid},
      rectangle_{rectangle} {}

Spriteset const& Character::characterset() const { return characterset_; }

Action Character::currentAction() const {
  // FIXME: On the necessity of MovingDirection?
  switch (moving_direction_) {
    case kLeft:
      return Action::kWalkLeft;
    case kDown:
      return Action::kWalkDown;
    case kRight:
      return Action::kWalkRight;
    case kUp:
      return Action::kWalkUp;
    default:
      return Action::kWalkLeft;  // FIXME.
  }
}

int64_t Character::height() const { return rectangle_.height(); }

void Character::moveBy(Vector const& desired_displacement) {
  Position final_position{
      navigator_.moveBy(positioned_solid_, desired_displacement)};
  updateMovingDirection(desired_displacement, final_position - position());
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

void Character::updateMovingDirection(Vector const& desired_displacement,
                                      Vector const& actual_displacement) {
  /* Using the desired displacement as fallback ensures that when the character
   * is blocked, it still turns toward the tried direction. */
  if (actual_displacement != Vector::kZero) {
    animation_tick_++;
    updateMovingDirectionForDisplacement(actual_displacement);
  } else {
    resetAnimationTick();
    updateMovingDirectionForDisplacement(desired_displacement);
  }
}

void Character::updateMovingDirectionForDisplacement(
    Vector const& displacement) {
  set<MovingDirection> moving_directions;
  if (displacement.dx() < 0) {
    moving_directions.insert(MovingDirection::kLeft);
  } else if (displacement.dx() > 0) {
    moving_directions.insert(MovingDirection::kRight);
  }

  if (displacement.dy() < 0) {
    moving_directions.insert(MovingDirection::kUp);
  } else if (displacement.dy() > 0) {
    moving_directions.insert(MovingDirection::kDown);
  }

  /* Favour the current moving direction if it is still active. Otherwise pick a
   * random direction. Because direction changes are sequential, picking a
   * "random" direction is always right. */
  if (!moving_directions.empty() &&
      !moving_directions.contains(moving_direction_)) {
    // The direction changed.
    moving_direction_ = *moving_directions.begin();
    resetAnimationTick();
  }
}

int64_t Character::width() const { return rectangle_.width(); }

int64_t Character::x() const { return position().x(); }

int64_t Character::y() const { return position().y(); }

vector<Character> CharacterLoader::load(
    string const& characters_file_path, vector<Spriteset> const& charactersets,
    unordered_map<int64_t, ActionSpriteMapper const> const&
        action_sprite_mappers,
    unordered_map<int64_t, AnimationPlayer const> const& animation_players,
    unordered_map<int64_t, Collider const> const& colliders,
    Navigator const& navigator, int64_t tiles_width, int64_t tiles_height) {
  vector<Character> characters;
  ifstream stream;
  stream.open(characters_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t x{0};
      int64_t y{0};
      uint16_t characterset_index{0};
      uint16_t action_sprite_mapper_index{0};
      uint16_t animations_index{0};
      uint16_t collision_index{0};
      uint8_t controller{0};  // FIXME: Make use.
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&x), kXFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&y), kYFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&characterset_index),
                  kCharactersetFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&action_sprite_mapper_index),
                  kIndicesFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&animations_index),
                  kAnimationFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&collision_index),
                  kCollisionFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&controller), kControllerFieldSize);
      characters.emplace_back(
          charactersets[characterset_index],
          action_sprite_mappers.at(action_sprite_mapper_index),
          animation_players.at(animations_index),
          colliders.at(collision_index).zero(), navigator,
          Position{x * tiles_width, y * tiles_height},
          Rectangle{16, 16});  // FIXME: From collisions.
    }
    stream.close();
  }  // FIXME: Raise exception.

  return characters;
}
