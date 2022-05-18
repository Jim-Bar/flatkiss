#ifndef POSITIONED_ELLIPSE_HPP_INCLUDED
#define POSITIONED_ELLIPSE_HPP_INCLUDED

#include "ellipse.hpp"
#include "position.hpp"

class PositionedEllipse {
 public:
  PositionedEllipse(Position const& position, Ellipse const& ellipse);
  PositionedEllipse(PositionedEllipse const& other);
  PositionedEllipse(PositionedEllipse&& other);
  ~PositionedEllipse();
  PositionedEllipse& operator=(PositionedEllipse const& other) = delete;
  PositionedEllipse& operator=(PositionedEllipse&& other) = delete;
  Ellipse const& ellipse() const;
  /**
   * @brief Returns the position of the center of the ellipse.
   *
   * @return Position const& the position of the center of the ellipse.
   */
  Position const& position() const;
  int64_t radiusX() const;
  int64_t radiusY() const;
  /**
   * @brief Returns the position on the X axis of the center of the ellipse.
   *
   * @return int64_t the X position of the center of the ellipse.
   */
  int64_t x() const;
  /**
   * @brief Returns the position on the Y axis of the center of the ellipse.
   *
   * @return int64_t the Y position of the center of the ellipse.
   */
  int64_t y() const;

 private:
  Ellipse const ellipse_;
  Position const position_;
};

#endif
