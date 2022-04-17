#ifndef COLLIDER_HPP_INCLUDED
#define COLLIDER_HPP_INCLUDED

#include <unordered_map>

#include "collision.hpp"

/**
 * @brief Collides objects with tile indices.
 */
class Collider {
 public:
  /**
   * @brief Constructs a Collider from a map of collisions.
   *
   * When this constructor returns, the passed map is empty because its values
   * have been transferred to the collider.
   *
   * @param collisions_per_tile_index Map of tile indices to their respective
   * collisions.
   */
  Collider(std::unordered_map<uint16_t, Collision>&&
               collisions_per_tile_index);
  /**
   * @brief Tells whether a positioned rectangle overlaps with the tile at the
   * given position.
   *
   * @param positioned_rectangle Positioned rectangle to collide with the tile.
   * @param tile_index Tile index of the tile at the given position.
   * @param tile_position Position of the tile.
   * @return true If the tile of given index and position collides with the
   * provided positioned rectangle.
   * @return false If there is no collision.
   */
  bool collide(PositionedRectangle const& positioned_rectangle,
               uint16_t tile_index, Position const& tile_position) const;

 private:
  std::unordered_map<uint16_t, Collision> const collisions_per_tile_index_;
};

/**
 * @brief Helper class for loading the collisions from a file.
 */
class ColliderLoader {
 public:
  static std::unordered_map<int64_t, Collider const> load(
      std::string const& file_path);

 private:
  static std::unordered_map<uint16_t, Collision> loadGroup(
      int64_t group_size, std::ifstream& collisions_stream);
};

#endif
