#include <fstream>

#include "Level.hpp"

using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;

Level::Level(unique_ptr<uint16_t const[]> Tiles, size_t WidthInTiles, size_t HeightInTiles) : Tiles(move(Tiles)), WidthInTiles(WidthInTiles), HeightInTiles(HeightInTiles) {

}

size_t Level::heightInTiles() const {
    return HeightInTiles;
}

uint16_t Level::tileIndex(size_t i, size_t j) const {
    return Tiles[j * WidthInTiles + i];
}

size_t Level::widthInTiles() const {
    return WidthInTiles;
}

unique_ptr<Level const> LevelLoader::load(string const& Path, size_t WidthInTiles, size_t HeightInTiles) {
    size_t const SizeInBytes{WidthInTiles * HeightInTiles * 2}; // Two byte per tile.
    auto Tiles{make_unique<uint16_t[]>(SizeInBytes)};
    std::ifstream Stream;
    Stream.open(Path, std::ios::in | std::ios::binary);
    if (Stream.is_open()) {
        Stream.read(reinterpret_cast<char*>(Tiles.get()), SizeInBytes);
        Stream.close();
    } // FIXME: fail.

    return make_unique<Level const>(move(Tiles), WidthInTiles, HeightInTiles);
}
