#include "character.hpp"

#include <fstream>

using std::ifstream;
using std::ios;
using std::istream;
using std::string;
using std::vector;

Character::Character(uint16_t characterset, Navigator const& navigator,
                     Position const& initialPosition,
                     Rectangle const& rectangle)
    : characterset_{characterset},
      navigator_{navigator},
      positioned_rectangle_{initialPosition, rectangle} {}

uint16_t Character::characterset() const { return characterset_; }

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

vector<Character> CharacterLoader::load(string const& characters_file_path,
                                        Navigator const& navigator,
                                        int64_t tiles_size) {
  vector<Character> characters;
  ifstream stream;
  stream.open(characters_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t x{0};
      int64_t y{0};
      uint16_t characterset{0};
      uint16_t animation{0};  // FIXME: Make use.
      uint16_t collision{0};  // FIXME: Make use.
      uint8_t controller{0};  // FIXME: Make use.
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&x), kXFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&y), kYFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&characterset),
                  kCharactersetFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&animation), kAnimationFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&collision), kCollisionFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&controller), kControllerFieldSize);
      characters.emplace_back(characterset, navigator,
                              Position{x * tiles_size, y * tiles_size},
                              Rectangle{16, 16});  // FIXME: From collisions.
    }
    stream.close();
  }  // FIXME: Raise exception.

  return characters;
}
