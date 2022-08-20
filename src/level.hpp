#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include <string>
#include <vector>

#include "spriteset.hpp"

/**
 * @brief Models a level.
 *
 * A level is a list of tiles indices, with dimensions. Once created, this class
 * provides handy methods to access the content of the level.
 */
class Level {
 public:
  Level(std::vector<uint16_t>&& tiles, int64_t width_in_tiles,
        int64_t height_in_tiles, Spriteset const& spriteset);
  int64_t heightInTiles() const;
  Spriteset const& spriteset() const;
  uint16_t tileIndex(int64_t i, int64_t j) const;
  int64_t widthInTiles() const;

 private:
  int64_t const height_in_tiles_;
  Spriteset const& spriteset_;
  std::vector<uint16_t> const tiles_;
  int64_t const width_in_tiles_;
};

/**
 * @brief Helper class for loading the level from a file.
 */
class LevelLoader {
 public:
  static std::vector<Level> load(std::string const& file_path,
                                 std::vector<Spriteset> const& spritesets);
};

#endif
