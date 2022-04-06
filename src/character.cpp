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
using std::vector;

Character::Character(Characterset const& characterset,
                     Navigator const& navigator,
                     Position const& initialPosition,
                     Rectangle const& rectangle)
    : characterset_{characterset},
      moving_direction_{MovingDirection::kDown},
      navigator_{navigator},
      positioned_rectangle_{initialPosition, rectangle} {}

int64_t Character::animationTick() const { return animation_tick_; }

Characterset const& Character::characterset() const { return characterset_; }

int64_t Character::height() const {
  return positioned_rectangle_.rectangle().height();
}

void Character::moveBy(Vector const& desired_displacement) {
  Position final_position{
      navigator_.moveBy(positioned_rectangle_, desired_displacement)};
  updateMovingDirection(desired_displacement, final_position - position());
  positioned_rectangle_.position(move(final_position));
}

MovingDirection const& Character::movingDirection() const {
  return moving_direction_;
}

Position const& Character::position() const {
  return positioned_rectangle_.position();
}

Rectangle const& Character::rectangle() const {
  return positioned_rectangle_.rectangle();
}

void Character::resetAnimationTick() {
  /* Not resetting to zero. Instead resetting to the last tick before which the
   * next animation is played. Consequently, next time the character moves, it
   * immediately starts animating. In particular, this prevents it from sliding
   * for small moves.*/
  animation_tick_ =
      characterset_.animationDurationForMovingDirection(moving_direction_) - 1;
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
      !moving_directions.contains(movingDirection())) {
    // The direction changed.
    moving_direction_ = *moving_directions.begin();
    resetAnimationTick();
  }
}

int64_t Character::width() const {
  return positioned_rectangle_.rectangle().width();
}

int64_t Character::x() const { return position().x(); }

int64_t Character::y() const { return position().y(); }

vector<Character> CharacterLoader::load(
    string const& characters_file_path,
    vector<Characterset> const& charactersets, Navigator const& navigator,
    int64_t tiles_size) {
  vector<Character> characters;
  ifstream stream;
  stream.open(characters_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t x{0};
      int64_t y{0};
      uint16_t characterset_index{0};
      uint16_t animation{0};  // FIXME: Make use.
      uint16_t collision{0};  // FIXME: Make use.
      uint8_t controller{0};  // FIXME: Make use.
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&x), kXFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&y), kYFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&characterset_index),
                  kCharactersetFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&animation), kAnimationFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&collision), kCollisionFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&controller), kControllerFieldSize);
      characters.emplace_back(charactersets[characterset_index], navigator,
                              Position{x * tiles_size, y * tiles_size},
                              Rectangle{16, 16});  // FIXME: From collisions.
    }
    stream.close();
  }  // FIXME: Raise exception.

  return characters;
}
