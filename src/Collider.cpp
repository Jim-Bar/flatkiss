#include "Collider.hpp"

Collider::Collider(
    std::unordered_map<uint16_t, Collision const>&& CollisionsPerTileIndex) {
  this->CollisionsPerTileIndex.swap(CollisionsPerTileIndex);
}

bool Collider::collide(PositionedRectangle const& PositionedRect,
                       uint16_t TileIndex, Position const& TilePosition) const {
  if (!CollisionsPerTileIndex.contains(TileIndex)) {
    return false;
  }

  Collision const& Collision{CollisionsPerTileIndex.at(TileIndex)};

  return Collision.collidesWith(PositionedRect, TilePosition);
}
