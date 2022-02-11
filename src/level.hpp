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
  Level(std::vector<uint16_t> const& tiles, size_t width_in_tiles,
        size_t height_in_tiles);
  size_t heightInTiles() const;
  uint16_t tileIndex(size_t i, size_t j) const;
  size_t widthInTiles() const;

 private:
  size_t const height_in_tiles_;
  std::vector<uint16_t> const tiles_;
  size_t const width_in_tiles_;
};

class LevelLoader {
 public:
  static std::unique_ptr<Level const> load(std::string const& path,
                                           size_t width_in_tiles,
                                           size_t height_in_tiles);
};

#endif
