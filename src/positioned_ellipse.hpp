#ifndef POSITIONED_ELLIPSE_HPP_INCLUDED
#define POSITIONED_ELLIPSE_HPP_INCLUDED

#include "ellipse.hpp"
#include "position.hpp"

class PositionedEllipse {
 public:
  PositionedEllipse(Position const& position, Ellipse const& ellipse);
  bool contains(Position const& position) const;
  Ellipse const& ellipse() const;
  Position const& position() const;
  size_t radiusX() const;
  size_t radiusY() const;
  size_t x() const;
  size_t y() const;

 private:
  Ellipse const ellipse_;
  Position const position_;
};

#endif