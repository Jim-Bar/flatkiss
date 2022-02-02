#ifndef NAVIGATOR_HPP_INCLUDED
#define NAVIGATOR_HPP_INCLUDED

#include "Collider.hpp"
#include "Level.hpp"
#include "PositionedRectangle.hpp"

/**
 * @brief Move a positioned rectangle to the closest desired location.
 */
class Navigator {
public:
    Navigator(Collider const& Collider, Level const& Level, size_t const TilesSize);
    Position navigateTo(PositionedRectangle const& SourcePositionedRectangle, Position const& Destination) const;

private:
    Collider const& TheCollider;
    Level const& TheLevel;
    size_t const TilesSize;
};

#endif
