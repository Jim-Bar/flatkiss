#include "Collider.hpp"

Collider::Collider(std::unordered_map<uint16_t, Collision const>&& CollisionsPerTileIndex) {
    this->CollisionsPerTileIndex.swap(CollisionsPerTileIndex);
}

bool Collider::collides(uint16_t TileIndex, size_t CharacterX, size_t CharacterY, size_t TileX, size_t TileY, size_t CharacterSize) const {
    if (!CollisionsPerTileIndex.contains(TileIndex)) {
        return false;
    }

    Collision const& Collision{CollisionsPerTileIndex.at(TileIndex)};

    // FIXME: Create the Position, Rectangle and such higher in the stack.
    return Collision.collidesWith(PositionedRectangle{Position{CharacterX, CharacterY}, Rectangle{CharacterSize, CharacterSize}}, Position{TileX, TileY});
}
