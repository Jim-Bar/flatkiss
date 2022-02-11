#ifndef POSITION_HPP_INCLUDED
#define POSITION_HPP_INCLUDED

#include <cstddef>

#include "Vector.hpp"

// Forward declaration to break the cycle Position / PositionedEllipse;
class PositionedEllipse;
// Forward declaration to break the cycle Position / PositionedRectangle.
class PositionedRectangle;

/**
 * @brief A position in pixels somewhere in the world, viewport or whatever.
 *
 * Note that this is always in pixels. This class must not be used for things
 * like location in the world in tiles.
 */
class Position {
 public:
  Position(size_t X, size_t Y);
  bool operator!=(Position const& Position) const;
  Position operator+(Position const& OtherPosition) const;
  /**
   * @brief Returns a new positioned ellipse moved by the position.
   * The position here can be considered as a vector, or a distance.
   *
   * @param PositionedEllipse The positioned ellipse that will be moved.
   * @return PositionedEllipse A new positioned ellipse moved by the current
   * position.
   */
  PositionedEllipse operator+(PositionedEllipse const& PositionedEllipse) const;
  /**
   * @brief Returns a new positioned rectangle moved by the position.
   * The position here can be considered as a vector, or a distance.
   *
   * @param PositionedRectangle The positioned rectangle that will be moved.
   * @return PositionedRectangle A new positioned rectangle moved by the current
   * position.
   */
  PositionedRectangle operator+(
      PositionedRectangle const& PositionedRectangle) const;
  Position operator+(Vector const& Vector) const;
  /**
   * @brief Returns a new delta between the two positions.
   *
   * @param Position The other position that will be substracted from the
   * current position.
   * @return Vector A new vector from the other position to the current one.
   */
  Vector operator-(Position const& Position) const;
  bool operator==(Position const& Position) const;
  size_t x() const;
  size_t y() const;

 private:
  size_t const X;
  size_t const Y;

  int64_t substractSizes(size_t Size1, size_t Size2) const;
};

#endif
