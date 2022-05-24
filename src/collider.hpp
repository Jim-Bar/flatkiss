#ifndef COLLIDER_HPP_INCLUDED
#define COLLIDER_HPP_INCLUDED

#include "positioned_ellipse.hpp"
#include "positioned_rectangle.hpp"
#include "positioned_solid.hpp"

/**
 * @brief Collides things.
 */
class Collider {
 public:
  static bool collide(PositionedEllipse const& ellipse1,
                      PositionedEllipse const& ellipse2);
  static bool collide(PositionedEllipse const& ellipse,
                      PositionedRectangle const& rectangle);
  static bool collide(PositionedRectangle const& rectangle,
                      PositionedEllipse const& ellipse);
  static bool collide(PositionedRectangle const& rectangle1,
                      PositionedRectangle const& rectangle2);
  static bool collide(PositionedSolid const& solid1,
                      PositionedSolid const& solid2);

 private:
  static bool collide(PositionedEllipse const& ellipse,
                      Position const& position);
};

#endif
