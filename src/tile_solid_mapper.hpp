#ifndef TILE_SOLID_MAPPER_HPP_INCLUDED
#define TILE_SOLID_MAPPER_HPP_INCLUDED

#include <cstdint>
#include <string>
#include <unordered_map>

// FIXME: Merge mappers in the same class?
/**
 * @brief Maps tile indices to solid indices.
 */
class TileSolidMapper {
 public:
  /**
   * @brief Construct a TileSolidMapper from a map of tile indices to matching
   * solid indices.
   *
   * @param tiles_to_solids Map of tile indices to solid indices.
   */
  TileSolidMapper(std::unordered_map<uint16_t, int64_t>&& tiles_to_solids);
  bool contains(uint16_t tile_index) const;
  int64_t solidIndexForTileIndex(uint16_t tile_index) const;

 private:
  std::unordered_map<uint16_t, int64_t> const tiles_to_solids_;
};

class TileSolidMapperLoader {
 public:
  static std::unordered_map<int64_t, TileSolidMapper const> load(
      std::string const& tile_solid_map_file_path);

 private:
  static std::unordered_map<uint16_t, int64_t> loadGroup(int64_t group_size,
                                                         std::ifstream& stream);
};

#endif
