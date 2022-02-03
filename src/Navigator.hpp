#ifndef NAVIGATOR_HPP_INCLUDED
#define NAVIGATOR_HPP_INCLUDED

#include "Collider.hpp"
#include "Level.hpp"
#include "PositionedRectangle.hpp"
#include "Vector.hpp"

/**
 * @brief Move a positioned rectangle to the closest desired location.
 */
class Navigator {
public:
    Navigator(Collider const& Collider, Level const& Level, size_t const TilesSize);
    Position moveBy(PositionedRectangle const& SourcePositionedRectangle, Vector const& DesiredDisplacement) const;

private:
    Collider const& TheCollider;
    Level const& TheLevel;
    size_t const TilesSize;

    size_t clampToLevelBounds(size_t ObjectPosition, size_t ObjectSize, int64_t DeltaValue, size_t LevelSize) const;
    bool collidesWithTiles(PositionedRectangle const& PositionedRectangle) const; // FIXME: Move to Collider?
    Position findNearestPositionToDestination(PositionedRectangle const& SourcePositionedRectangle, Position const& Destination) const;
};

#endif
