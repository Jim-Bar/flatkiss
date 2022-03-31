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

CharacterFamily::CharacterFamily(uint16_t characterset, int64_t width,
                                 int64_t height)
    : characterset_{characterset}, width_{width}, height_{height} {}

uint16_t CharacterFamily::characterset() const { return characterset_; }

int64_t CharacterFamily::height() const { return height_; }

int64_t CharacterFamily::width() const { return width_; }

vector<Character> CharacterLoader::load(
    string const& characters_file_path,
    string const& characters_families_file_path, Navigator const& navigator,
    int64_t tiles_size) {
  vector<CharacterFamily> families{loadFamilies(characters_families_file_path)};
  vector<Character> characters;
  ifstream stream;
  stream.open(characters_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      int64_t x{0};
      int64_t y{0};
      uint16_t character_family{0};
      uint8_t controller{0};  // FIXME: Make use.
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&x), kXFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&y), kYFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&character_family),
                  kCharacterSetFieldSize);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&controller), kControllerFieldSize);
      characters.emplace_back(families[character_family].characterset(),
                              navigator,
                              Position{x * tiles_size, y * tiles_size},
                              Rectangle{families[character_family].width(),
                                        families[character_family].height()});
    }
    stream.close();
  }  // FIXME: Raise exception.

  return characters;
}

vector<CharacterFamily> CharacterLoader::loadFamilies(
    string const& characters_families_file_path) {
  vector<CharacterFamily> characters_families;
  ifstream stream;
  stream.open(characters_families_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      uint16_t characterset{0};
      uint8_t width{0};
      uint8_t height{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&characterset), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&width), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&height), 1);
      characters_families.emplace_back(characterset, width, height);
    }
    stream.close();
  }  // FIXME: Raise exception.

  return characters_families;
}
