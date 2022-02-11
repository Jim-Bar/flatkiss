#include "collider.hpp"

Collider::Collider(
    std::unordered_map<uint16_t, Collision const>&& collisions_per_tile_index) {
  this->collisions_per_tile_index_.swap(collisions_per_tile_index);
}

bool Collider::collide(PositionedRectangle const& positioned_rectangle,
                       uint16_t tile_index,
                       Position const& tile_position) const {
  if (!collisions_per_tile_index_.contains(tile_index)) {
    return false;
  }

  Collision const& collision{collisions_per_tile_index_.at(tile_index)};

  return collision.collidesWith(positioned_rectangle, tile_position);
}
