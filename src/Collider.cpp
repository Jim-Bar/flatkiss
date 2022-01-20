#include "Collider.hpp"

Collider::Collider(std::unordered_map<uint16_t, Collision const>&& CollisionsPerTileIndex) {
    this->CollisionsPerTileIndex.swap(CollisionsPerTileIndex);
}

bool Collider::collides(uint16_t TileIndex, size_t CharacterX, size_t CharacterY, size_t TileX, size_t TileY, size_t CharacterSize) const {
    if (CollisionsPerTileIndex.find(TileIndex) == CollisionsPerTileIndex.end()) { // FIXME: Use contains()
        return false;
    }
/*
    Collision const& Collision{CollisionsPerTileIndex.at(TileIndex)};

    if (CharacterX + CharacterSize <= TileX + Collision.x()) {
        return false;
    }

    if (CharacterX > TileX + Collision.x() + Collision.width()) {
        return false;
    }

    if (CharacterY + CharacterSize <= TileY + Collision.y()) {
        return false;
    }

    if (CharacterY > TileY + Collision.y() + Collision.height()) {
        return false;
    }*/

    return true;
}
