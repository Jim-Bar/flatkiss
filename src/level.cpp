#include "level.hpp"

#include <fstream>

using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;

Level::Level(vector<uint16_t> const& tiles, size_t width_in_tiles,
             size_t height_in_tiles)
    : tiles_(move(tiles)),
      width_in_tiles_(width_in_tiles),
      height_in_tiles_(height_in_tiles) {}

size_t Level::heightInTiles() const { return height_in_tiles_; }

uint16_t Level::tileIndex(size_t i, size_t j) const {
  return tiles_[j * width_in_tiles_ + i];
}

size_t Level::widthInTiles() const { return width_in_tiles_; }

unique_ptr<Level const> LevelLoader::load(string const& path,
                                          size_t width_in_tiles,
                                          size_t height_in_tiles) {
  // Two byte per tile.
  size_t const size_in_bytes{width_in_tiles * height_in_tiles * 2};
  auto tiles{vector<uint16_t>(size_in_bytes, 0)};
  std::ifstream stream;
  stream.open(path, std::ios::in | std::ios::binary);
  if (stream.is_open()) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(tiles.data()), size_in_bytes);
    stream.close();
  }  // FIXME: fail.

  return make_unique<Level const>(move(tiles), width_in_tiles, height_in_tiles);
}