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
  Navigator(Collider const& Collider, Level const& Level,
            size_t const TilesSize);
  Position moveBy(PositionedRectangle const& SourcePositionedRectangle,
                  Vector const& DesiredDisplacement) const;

 private:
  Collider const& TheCollider;
  Level const& TheLevel;
  size_t const TilesSize;

  /**
   * @brief Given a position and a movement (delta) on an axis (representing
   * either X or Y), return the resulting position taking account the bounds.
   *
   * @param ObjectPosition Position on the axis.
   * @param ObjectSize Size of the object at the given position.
   * @param DeltaValue Movement of the object.
   * @param UpperBound Maximum position on the axis (the minimum position is
   * zero).
   * @return size_t The resulting position of the object on the axis after
   * applying the movement, and clamping to the bounds.
   */
  size_t clampToBounds(size_t ObjectPosition, size_t ObjectSize,
                       int64_t DeltaValue, size_t UpperBound) const;
  bool collidesWithTiles(PositionedRectangle const& PositionedRectangle)
      const;  // FIXME: Move to Collider?
  Position findNearestPositionToDestination(
      PositionedRectangle const& SourcePositionedRectangle,
      Position const& Destination) const;
};

#endif
