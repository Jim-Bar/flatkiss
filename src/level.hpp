#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include <memory>
#include <string>
#include <vector>

/**
 * @brief Models a level.
 *
 * A level is a list of tiles indices, with dimensions. Once created, this class
 * provides handy methods to access the content of the level.
 */
class Level {
 public:
  Level(std::vector<uint16_t> const& tiles, int64_t width_in_tiles,
        int64_t height_in_tiles);
  int64_t heightInTiles() const;
  uint16_t tileIndex(int64_t i, int64_t j) const;
  int64_t widthInTiles() const;

 private:
  int64_t const height_in_tiles_;
  std::vector<uint16_t> const tiles_;
  int64_t const width_in_tiles_;
};

class LevelLoader {
 public:
  static std::unique_ptr<Level const> load(std::string const& path,
                                           int64_t width_in_tiles,
                                           int64_t height_in_tiles);
};

#endif
