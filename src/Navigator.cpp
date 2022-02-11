#include "Navigator.hpp"

#include <algorithm>

Navigator::Navigator(Collider const& Collider, Level const& Level,
                     size_t const TilesSize)
    : TheCollider(Collider), TheLevel(Level), TilesSize(TilesSize) {}

size_t Navigator::clampToBounds(size_t ObjectPosition, size_t ObjectSize,
                                int64_t DeltaValue, size_t UpperBound) const {
  // Here care is taken to deal with unsigned integers and substractions.
  if (DeltaValue < 0 && ObjectPosition < -DeltaValue) {
    return 0;
  } else if (DeltaValue > 0 &&
             ObjectPosition + ObjectSize + DeltaValue >= UpperBound) {
    return UpperBound - ObjectSize;
  } else {
    return ObjectPosition + DeltaValue;
  }
}

bool Navigator::collidesWithTiles(
    PositionedRectangle const& PositionedRectangle) const {
  for (size_t Y(PositionedRectangle.y() / TilesSize);
       Y <=
       (PositionedRectangle.y() + PositionedRectangle.height() - 1) / TilesSize;
       Y++) {
    for (size_t X(PositionedRectangle.x() / TilesSize);
         X <= (PositionedRectangle.x() + PositionedRectangle.width() - 1) /
                  TilesSize;
         X++) {
      uint16_t TileIndex(TheLevel.tileIndex(X, Y));
      if (TheCollider.collide(PositionedRectangle, TileIndex,
                              Position{X * TilesSize, Y * TilesSize})) {
        return true;
      }
    }
  }

  return false;
}

Position Navigator::findNearestPositionToDestination(
    PositionedRectangle const& SourcePositionedRectangle,
    Position const& Destination) const {
  /* Decompose the displacement in steps. Each step is a point. Because the
   * components of the displacement can be different, first find the greatest of
   * the two. This is the number of steps. Then move step by step (point by
   * point). On the first collision, return the last position (it did not
   * collide). This is the nearest position to the destination, and which does
   * not collide. Note that this implementation find the nearest position on the
   * line between the source and the destination. It will not return the actual
   * nearest position when it is outside of that line. */
  Vector Displacement{Destination - SourcePositionedRectangle.position()};
  int64_t MaxDisplacement{
      std::max(std::abs(Displacement.dx()), std::abs(Displacement.dy()))};
  for (int64_t Step{1}; Step <= MaxDisplacement; Step++) {
    Vector PartialDisplacement{(Step * Displacement.dx()) / MaxDisplacement,
                               (Step * Displacement.dy()) / MaxDisplacement};
    if (collidesWithTiles(SourcePositionedRectangle + PartialDisplacement)) {
      /* Return the last step for which the position does not collide (for the
       * first step this is the original position). */
      return Position{SourcePositionedRectangle.x() +
                          ((Step - 1) * Displacement.dx()) / MaxDisplacement,
                      SourcePositionedRectangle.y() +
                          ((Step - 1) * Displacement.dy()) / MaxDisplacement};
    }
  }

  /* FIXME: Raise an exception (this must only be called when the destination
   * cannot be reached). */
  return Destination;
}

Position Navigator::moveBy(PositionedRectangle const& SourcePositionedRectangle,
                           Vector const& DesiredDisplacement) const {
  /* First collide with the bounds of the level. Compute the resulting
   * (potential) destination. */
  Position Destination{
      clampToBounds(SourcePositionedRectangle.x(),
                    SourcePositionedRectangle.width(), DesiredDisplacement.dx(),
                    TheLevel.widthInTiles() * TilesSize),
      clampToBounds(
          SourcePositionedRectangle.y(), SourcePositionedRectangle.height(),
          DesiredDisplacement.dy(), TheLevel.heightInTiles() * TilesSize)};

  /* Secondly, if the destination is the same as the current position, nothing
   * to do. */
  if (SourcePositionedRectangle.position() == Destination) {
    return SourcePositionedRectangle.position();
  }

  /* Otherwise if there is a collision with a tile, make sure to stick to the
   * tile. */
  if (collidesWithTiles(PositionedRectangle{
          Destination, SourcePositionedRectangle.rectangle()})) {
    Position NearestPosition{findNearestPositionToDestination(
        SourcePositionedRectangle, Destination)};
    if (SourcePositionedRectangle.position() != NearestPosition) {
      return NearestPosition;
    }

    // Maybe it is possible to slide against the obstacle along the X axis.
    Position DestinationX{Destination.x(), SourcePositionedRectangle.y()};
    if (collidesWithTiles(PositionedRectangle{
            DestinationX, SourcePositionedRectangle.rectangle()})) {
      Position NearestPositionX{findNearestPositionToDestination(
          SourcePositionedRectangle, DestinationX)};
      if (SourcePositionedRectangle.position() != NearestPositionX) {
        return NearestPositionX;
      }
    } else {
      return DestinationX;
    }

    // Or along the Y axis.
    Position DestinationY{SourcePositionedRectangle.x(), Destination.y()};
    if (collidesWithTiles(PositionedRectangle{
            DestinationY, SourcePositionedRectangle.rectangle()})) {
      Position NearestPositionY{findNearestPositionToDestination(
          SourcePositionedRectangle, DestinationY)};
      if (SourcePositionedRectangle.position() != NearestPositionY) {
        return NearestPositionY;
      }
    } else {
      return DestinationY;
    }

    // It is not possible to get to a nearer position.
    return SourcePositionedRectangle.position();
  }

  // But if there is not collision, just go to the final destination.
  return Destination;
}
