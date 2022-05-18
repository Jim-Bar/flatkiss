#ifndef POSITIONED_RECTANGLE_HPP_INCLUDED
#define POSITIONED_RECTANGLE_HPP_INCLUDED

#include "position.hpp"
#include "rectangle.hpp"
#include "vector.hpp"

/**
 * @brief A rectangle with a position.
 */
class PositionedRectangle {
 public:
  PositionedRectangle(Position const& position, Rectangle const& rectangle);
  PositionedRectangle(PositionedRectangle const& other);
  PositionedRectangle(PositionedRectangle&& other);
  ~PositionedRectangle();
  PositionedRectangle& operator=(PositionedRectangle const& other) = delete;
  PositionedRectangle& operator=(PositionedRectangle&& other) = delete;
  int64_t height() const;
  /**
   * @brief Returns a new positioned rectangle moved by the provided
   * displacement.
   *
   * @param vector The displacement to apply.
   * @return PositionedRectangle A new positioned rectangle moved by the vector.
   */
  PositionedRectangle operator+(Vector const& vector) const;
  Position const& position() const;
  void position(Position&& new_position);
  Rectangle const& rectangle() const;
  int64_t width() const;
  int64_t x() const;
  void x(int64_t newX);
  int64_t y() const;
  void y(int64_t newY);

 private:
  Position position_;
  Rectangle const rectangle_;
};

#endif
