#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <vector>

#include "navigator.hpp"
#include "positioned_rectangle.hpp"

class Character {
 public:
  Character(Navigator const& navigator, Position const& initialPosition,
            Rectangle const& rectangle);
  int64_t height() const;
  void moveBy(Vector const& desired_displacement);
  Position const& position() const;
  Rectangle const& rectangle() const;
  int64_t width() const;
  int64_t x() const;
  int64_t y() const;

 private:
  Navigator const& navigator_;
  PositionedRectangle positioned_rectangle_;
};

/**
 * @brief Helper class for loading the characters from a file.
 */
class CharacterLoader {
 public:
  static std::vector<Character> load(std::string const& file_path,
                                     Navigator const& navigator,
                                     int64_t tiles_size);

 private:
  static int64_t constexpr kCharacterSetFieldSize{2};
  static int64_t constexpr kControllerFieldSize{1};
  static int64_t constexpr kXFieldSize{8};
  static int64_t constexpr kYFieldSize{8};
};

#endif
