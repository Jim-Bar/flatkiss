#ifndef POSITIONED_RECTANGLE_HPP_INCLUDED
#define POSITIONED_RECTANGLE_HPP_INCLUDED

#include "position.hpp"
#include "rectangle.hpp"
#include "vector.hpp"

/**
 * @brief A rectangle with a position.
 *
 * Provides useful routines such as intersection helpers.
 */
class PositionedRectangle {
 public:
  PositionedRectangle(Position const& position, Rectangle const& rectangle);
  PositionedRectangle(PositionedRectangle const& other);
  PositionedRectangle(PositionedRectangle&& other);
  ~PositionedRectangle();
  PositionedRectangle& operator=(PositionedRectangle const& other) = delete;
  PositionedRectangle& operator=(PositionedRectangle&& other) = delete;
  size_t height() const;
  bool intersectsWith(
      PositionedEllipse const& other) const;  // FIXME: Move to Collider?
  bool intersectsWith(PositionedRectangle const& other) const;
  /**
   * @brief Returns a new positioned rectangle moved by the provided
   * displacement.
   *
   * @param vector The displacement to apply.
   * @return PositionedRectangle A new positioned rectangle moved by the vector.
   */
  PositionedRectangle operator+(Vector const& vector) const;
  Position const& position() const;
  void position(Position&& newPosition);
  Rectangle const& rectangle() const;
  size_t width() const;
  size_t x() const;
  void x(size_t newX);
  size_t y() const;
  void y(size_t newY);

 private:
  Position position_;
  Rectangle const rectangle_;
};

#endif
