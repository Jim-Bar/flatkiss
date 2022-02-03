#include <algorithm>

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

bool Navigator::collidesWithTiles(PositionedRectangle const& PositionedRectangle) const {
    for (size_t Y(PositionedRectangle.y() / TilesSize); Y <= (PositionedRectangle.y() + PositionedRectangle.height()) / TilesSize; Y++) {
        for (size_t X(PositionedRectangle.x() / TilesSize); X <= (PositionedRectangle.x() + PositionedRectangle.width()) / TilesSize; X++) {
            uint16_t TileIndex(TheLevel.tileIndex(X, Y));
            if (TheCollider.collides(PositionedRectangle, TileIndex, Position{X * TilesSize, Y * TilesSize})) {
                return true;
            }
        }
    }

    return false;
}

Position Navigator::findNearestPositionToDestination(PositionedRectangle const& SourcePositionedRectangle, Position const& Destination) const {
    Vector Displacement{Destination - SourcePositionedRectangle.position()};
    size_t MaxDisplacement{std::max(std::abs(Displacement.dx()), std::abs(Displacement.dy()))}; // FIXME: Narrowing conversion.
    for (size_t Step{1}; Step <= MaxDisplacement; Step++) {
        int64_t DX{static_cast<int64_t>(static_cast<double>(Step * Displacement.dx()) / static_cast<double>(MaxDisplacement))};
        int64_t DY{static_cast<int64_t>(static_cast<double>(Step * Displacement.dy()) / static_cast<double>(MaxDisplacement))};
        Position NewStepPosition{SourcePositionedRectangle.x() + DX, SourcePositionedRectangle.y() + DY};
        if (collidesWithTiles(PositionedRectangle{NewStepPosition, SourcePositionedRectangle.rectangle()})) {
            // Return the last step for which the position does not collide (for the first step this is the original position).
            // TODO.
        }
    }

    // FIXME: Raise an exception (this must only be called when the destination cannot be reached).
    return SourcePositionedRectangle.position();
}

Position Navigator::moveBy(PositionedRectangle const& SourcePositionedRectangle, Vector const& DesiredDisplacement) const {
    Position Destination{clampToLevelBounds(SourcePositionedRectangle.x(), SourcePositionedRectangle.width(), DesiredDisplacement.dx(), TheLevel.widthInTiles()),
        clampToLevelBounds(SourcePositionedRectangle.y(), SourcePositionedRectangle.height(), DesiredDisplacement.dy(), TheLevel.heightInTiles())};

    if (SourcePositionedRectangle.x() == Destination.x() && SourcePositionedRectangle.y() == Destination.y()) {
        return SourcePositionedRectangle.position();
    }

    if (collidesWithTiles(PositionedRectangle{Destination, SourcePositionedRectangle.rectangle()})) {
        // FIXME: call findNearestPositionToDestination().
        return SourcePositionedRectangle.position();
    }

    return Destination;
}
