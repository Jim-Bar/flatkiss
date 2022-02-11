#include "level.hpp"

#include <fstream>

using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;

Level::Level(vector<uint16_t> const& Tiles, size_t WidthInTiles,
             size_t HeightInTiles)
    : Tiles(move(Tiles)),
      WidthInTiles(WidthInTiles),
      HeightInTiles(HeightInTiles) {}

size_t Level::heightInTiles() const { return HeightInTiles; }

uint16_t Level::tileIndex(size_t i, size_t j) const {
  return Tiles[j * WidthInTiles + i];
}

size_t Level::widthInTiles() const { return WidthInTiles; }

unique_ptr<Level const> LevelLoader::load(string const& Path,
                                          size_t WidthInTiles,
                                          size_t HeightInTiles) {
  // Two byte per tile.
  size_t const SizeInBytes{WidthInTiles * HeightInTiles * 2};
  auto Tiles{vector<uint16_t>(SizeInBytes, 0)};
  std::ifstream Stream;
  Stream.open(Path, std::ios::in | std::ios::binary);
  if (Stream.is_open()) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    Stream.read(reinterpret_cast<char*>(Tiles.data()), SizeInBytes);
    Stream.close();
  }  // FIXME: fail.

  return make_unique<Level const>(move(Tiles), WidthInTiles, HeightInTiles);
}
