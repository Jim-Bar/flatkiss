#include "character.hpp"

Character::Character(Navigator const& navigator,
                     Position const& initialPosition,
                     Rectangle const& rectangle)
    : navigator_{navigator},
      positioned_rectangle_{initialPosition, rectangle} {}

int64_t Character::height() const {
  return positioned_rectangle_.rectangle().height();
}

void Character::moveBy(Vector const& desired_displacement) {
  positioned_rectangle_.position(
      navigator_.moveBy(positioned_rectangle_, desired_displacement));
}

Position const& Character::position() const {
  return positioned_rectangle_.position();
}

Rectangle const& Character::rectangle() const {
  return positioned_rectangle_.rectangle();
}

int64_t Character::width() const {
  return positioned_rectangle_.rectangle().width();
}

int64_t Character::x() const { return position().x(); }

int64_t Character::y() const { return position().y(); }
