#include "Navigator.hpp"

Navigator::Navigator(Collider const& Collider, Level const& Level, size_t const TilesSize) : TheCollider(Collider), TheLevel(Level), TilesSize(TilesSize) {

}

size_t Navigator::clampToLevelBounds(size_t ObjectPosition, size_t ObjectSize, int64_t DeltaValue, size_t LevelSize) const {
    // Here care is taken to deal with unsigned integers and substractions.
    if (DeltaValue < 0 && ObjectPosition < -DeltaValue) {
        return 0;
    } else if (DeltaValue > 0 && ObjectPosition + ObjectSize + DeltaValue >= LevelSize * TilesSize) {
        return LevelSize * TilesSize - ObjectSize;
    } else {
        return ObjectPosition + DeltaValue;
    }
}

Position Navigator::navigateTo(PositionedRectangle const& SourcePositionedRectangle, Vector const& Displacement) const {
    Position Destination{clampToLevelBounds(SourcePositionedRectangle.x(), SourcePositionedRectangle.width(), Displacement.dx(), TheLevel.widthInTiles()),
        clampToLevelBounds(SourcePositionedRectangle.y(), SourcePositionedRectangle.height(), Displacement.dy(), TheLevel.heightInTiles())};

    if (SourcePositionedRectangle.x() == Destination.x() && SourcePositionedRectangle.y() == Destination.y()) {
        return SourcePositionedRectangle.position();
    }

    for (size_t Y(Destination.y() / TilesSize); Y <= (Destination.y() + SourcePositionedRectangle.height()) / TilesSize; Y++) {
        for (size_t X(Destination.x() / TilesSize); X <= (Destination.x() + SourcePositionedRectangle.width()) / TilesSize; X++) {
            uint16_t TileIndex(TheLevel.tileIndex(X, Y));
            if (TheCollider.collides(PositionedRectangle{Destination, Rectangle{SourcePositionedRectangle.width(), SourcePositionedRectangle.height()}}, TileIndex, Position{X * TilesSize, Y * TilesSize})) {
                // FIXME: Implement properly.
                return SourcePositionedRectangle.position();
            }
        }
    }

    return Destination;
}
