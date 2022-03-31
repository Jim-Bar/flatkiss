#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <vector>

#include "navigator.hpp"
#include "positioned_rectangle.hpp"

/**
 * @brief A character in the level.
 *
 * Can be seen as an instance of a character family.
 */
class Character {
 public:
  Character(uint16_t characterset, Navigator const& navigator,
            Position const& initialPosition, Rectangle const& rectangle);
  uint16_t characterset() const;
  int64_t height() const;
  void moveBy(Vector const& desired_displacement);
  Position const& position() const;
  Rectangle const& rectangle() const;
  int64_t width() const;
  int64_t x() const;
  int64_t y() const;

 private:
  uint16_t characterset_;
  Navigator const& navigator_;
  PositionedRectangle positioned_rectangle_;
};

/**
 * @brief Type or kind or category of a character.
 *
 * Only used internally for loading from disk and creating all the characters.
 */
// FIXME: Nested class.
class CharacterFamily {
 public:
  CharacterFamily(uint16_t characterset, int64_t width, int64_t height);
  uint16_t characterset() const;
  int64_t height() const;
  int64_t width() const;

 private:
  uint16_t const characterset_;
  int64_t const height_;
  int64_t const width_;
};

/**
 * @brief Helper class for loading the characters from a file.
 */
class CharacterLoader {
 public:
  static std::vector<Character> load(
      std::string const& characters_file_path,
      std::string const& characters_families_file_path,
      Navigator const& navigator, int64_t tiles_size);

 private:
  static int64_t constexpr kCharacterSetFieldSize{2};
  static int64_t constexpr kControllerFieldSize{1};
  static int64_t constexpr kXFieldSize{8};
  static int64_t constexpr kYFieldSize{8};

  static std::vector<CharacterFamily> loadFamilies(
      std::string const& characters_families_file_path);
};

#endif
