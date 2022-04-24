#include "tile_solid_mapper.hpp"

#include <fstream>
#include <utility>

using std::forward_as_tuple;
using std::ifstream;
using std::ios;
using std::istream;
using std::move;
using std::piecewise_construct;
using std::string;
using std::unordered_map;

TileSolidMapper::TileSolidMapper(
    unordered_map<uint16_t, int64_t>&& tiles_to_solids)
    : tiles_to_solids_{move(tiles_to_solids)} {}

bool TileSolidMapper::contains(uint16_t tile_index) const {
  return tiles_to_solids_.contains(tile_index);
}

int64_t TileSolidMapper::solidIndexForTileIndex(uint16_t tile_index) const {
  if (tiles_to_solids_.contains(tile_index)) {
    return tiles_to_solids_.at(tile_index);
  }

  return 0;  // FIXME: Raise exception.
}

unordered_map<int64_t, TileSolidMapper const> TileSolidMapperLoader::load(
    string const& tile_solid_map_file_path) {
  unordered_map<int64_t, TileSolidMapper const> index_to_mapper;
  ifstream stream;
  stream.open(tile_solid_map_file_path, ios::in | ios::binary);
  if (stream.is_open()) {
    while (stream.peek() != istream::traits_type::eof()) {
      uint16_t group_index{0};
      uint16_t group_size{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&group_index), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      stream.read(reinterpret_cast<char*>(&group_size), 2);
      index_to_mapper.emplace(
          piecewise_construct, forward_as_tuple(group_index),
          forward_as_tuple(move(loadGroup(group_size, stream))));
    }
    stream.close();
  }  // FIXME: Raise exception.

  return index_to_mapper;
}

unordered_map<uint16_t, int64_t> TileSolidMapperLoader::loadGroup(
    int64_t group_size, std::ifstream& stream) {
  unordered_map<uint16_t, int64_t> tiles_to_solids;
  for (int64_t i{0}; i < group_size; i++) {
    uint16_t tile_index{0};
    uint16_t solid_index{0};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(&tile_index), 2);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stream.read(reinterpret_cast<char*>(&solid_index), 2);
    tiles_to_solids[tile_index] = solid_index;
  }

  return tiles_to_solids;
}
