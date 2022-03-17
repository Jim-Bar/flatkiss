#ifndef MOVABLE_POSITIONED_RECTANGLE_HPP_INCLUDED
#define MOVABLE_POSITIONED_RECTANGLE_HPP_INCLUDED

#include "positioned_rectangle.hpp"

/**
 * @brief A PositionedRectangle which can be moved.
 */
class MovablePositionedRectangle : public PositionedRectangle {
 public:
  MovablePositionedRectangle(Position const& position,
                             Rectangle const& rectangle);
  void moveTo(Position const& position);
  void moveTo(Position&& position);
  void setX(size_t x);
  void setY(size_t y);
};

#endif
