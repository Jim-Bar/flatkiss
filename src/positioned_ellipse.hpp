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
  bool contains(Position const& position) const;
  Ellipse const& ellipse() const;
  Position const& position() const;
  int64_t radiusX() const;
  int64_t radiusY() const;
  int64_t x() const;
  int64_t y() const;

 private:
  Ellipse const ellipse_;
  Position const position_;
};

#endif
