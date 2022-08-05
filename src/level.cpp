#include "level.hpp"

#include <fstream>
#include <utility>

using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::streamsize;
using std::string;
using std::vector;

Level::Level(vector<uint16_t>&& tiles, int64_t width_in_tiles,
             int64_t height_in_tiles, int64_t spriteset_index)
    : tiles_{move(tiles)},
      width_in_tiles_{width_in_tiles},
      height_in_tiles_{height_in_tiles},
      spriteset_index_{spriteset_index} {}

int64_t Level::heightInTiles() const { return height_in_tiles_; }

int64_t Level::spritesetIndex() const { return spriteset_index_; }

uint16_t Level::tileIndex(int64_t i, int64_t j) const {
  return tiles_[j * width_in_tiles_ + i];
}

int64_t Level::widthInTiles() const { return width_in_tiles_; }

vector<Level> LevelLoader::load(string const& file_path) {
  vector<Level> levels;
  ifstream stream;
  stream.open(file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      uint16_t width_in_tiles{0};
      uint16_t height_in_tiles{0};
      uint16_t spriteset_index{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&width_in_tiles), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&height_in_tiles), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&spriteset_index), 2);
      // Two bytes per tile.
      int64_t const size_in_bytes{width_in_tiles * height_in_tiles * 2};
      auto tiles{vector<uint16_t>(size_in_bytes, 0)};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(tiles.data()),
                  static_cast<streamsize>(size_in_bytes));
      levels.emplace_back(move(tiles), width_in_tiles, height_in_tiles,
                          spriteset_index);
    }
    stream.close();
  }  // FIXME: fail.

  return levels;
}
