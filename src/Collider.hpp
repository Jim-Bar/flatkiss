#ifndef COLLIDER_HPP_INCLUDED
#define COLLIDER_HPP_INCLUDED

#include <unordered_map>

#include "Collision.hpp"

/**
 * @brief Collides objects with tile indices.
 */
class Collider {
public:
    /**
     * @brief Constructs a Collider from a map of collisions.
     *
     * When this constructor returns, the passed map is empty because its values have been transferred to the collider.
     * 
     * @param CollisionsPerTileIndex Map of tile indices to their respective collisions.
     */
    Collider(std::unordered_map<uint16_t, Collision const>&& CollisionsPerTileIndex);
    /**
     * @brief Tells whether a positioned rectangle overlaps with the tile at the given position.
     * 
     * @param PositionedRect Positioned rectangle to collide with the tile.
     * @param TileIndex Tile index of the tile at the given position.
     * @param TilePosition Position of the tile.
     * @return true If the tile of given index and position collides with the provided positioned rectangle.
     * @return false If there is no collision.
     */
    bool collides(PositionedRectangle const& PositionedRect, uint16_t TileIndex, Position const& TilePosition) const;

private:
    std::unordered_map<uint16_t, Collision const> CollisionsPerTileIndex;
};

#endif
