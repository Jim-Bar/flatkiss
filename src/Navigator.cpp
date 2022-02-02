#include "Navigator.hpp"

Navigator::Navigator(Collider const& Collider, Level const& Level, size_t const TilesSize) : TheCollider(Collider), TheLevel(Level), TilesSize(TilesSize) {

}

Position Navigator::navigateTo(PositionedRectangle const& SourcePositionedRectangle, Position const& Destination) const {
    for (size_t Y(Destination.y() / TilesSize); Y <= (Destination.y() + SourcePositionedRectangle.height()) / TilesSize; Y++) {
        for (size_t X(Destination.x() / TilesSize); X <= (Destination.x() + SourcePositionedRectangle.width()) / TilesSize; X++) {
            uint16_t TileIndex(TheLevel.tileIndex(X, Y));
            if (TheCollider.collides(PositionedRectangle{Destination, Rectangle{SourcePositionedRectangle.width(), SourcePositionedRectangle.height()}}, TileIndex, Position{X * TilesSize, Y * TilesSize})) {
                // FIXME: Implement properly.
                return Position{SourcePositionedRectangle.x(), SourcePositionedRectangle.y()};
            }
        }
    }

    return Destination;
}
