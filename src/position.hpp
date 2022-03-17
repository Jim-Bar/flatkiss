#ifndef POSITION_HPP_INCLUDED
#define POSITION_HPP_INCLUDED

#include <cstddef>

#include "vector.hpp"

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
  Position(size_t x, size_t y);
  Position(Position const& other);
  Position(Position&& other);
  Position& operator=(Position const& other) = delete;
  Position& operator=(Position&& other) = delete;
  bool operator!=(Position const& other) const;
  Position operator+(Position const& other) const;
  /**
   * @brief Returns a new positioned ellipse moved by the position.
   * The position here can be considered as a vector, or a distance.
   *
   * @param ellipse The positioned ellipse that will be moved.
   * @return PositionedEllipse A new positioned ellipse moved by the current
   * position.
   */
  PositionedEllipse operator+(PositionedEllipse const& ellipse) const;
  /**
   * @brief Returns a new positioned rectangle moved by the position.
   * The position here can be considered as a vector, or a distance.
   *
   * @param rectangle The positioned rectangle that will be moved.
   * @return PositionedRectangle A new positioned rectangle moved by the current
   * position.
   */
  PositionedRectangle operator+(PositionedRectangle const& rectangle) const;
  Position operator+(Vector const& vector) const;
  /**
   * @brief Returns a new delta between the two positions.
   *
   * @param other The other position that will be substracted from the
   * current position.
   * @return Vector A new vector from the other position to the current one.
   */
  Vector operator-(Position const& other) const;
  bool operator==(Position const& other) const;
  size_t x() const;
  size_t y() const;

 protected:
  size_t x_;
  size_t y_;

 private:
  static int64_t substractSizes(size_t size_1, size_t size_2);
};

#endif
