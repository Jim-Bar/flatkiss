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
     * @brief Construct a Collider from a map of collisions.
     *
     * When this constructor returns, the passed map is empty because its values have been transferred to the collider.
     * 
     * @param CollisionsPerTileIndex Map of tile indices to their respective collisions.
     */
    Collider(std::unordered_map<uint16_t, Collision const>&& CollisionsPerTileIndex);
    bool collides(uint16_t TileIndex, size_t CharacterX, size_t CharacterY, size_t TileX, size_t TileY, size_t CharacterSize) const;

private:
    std::unordered_map<uint16_t, Collision const> CollisionsPerTileIndex;
};

#endif
