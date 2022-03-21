#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include "navigator.hpp"
#include "positioned_rectangle.hpp"

class Character {
 public:
  Character(Navigator const& navigator, Position const& initialPosition,
            Rectangle const& rectangle);
  size_t height() const;
  void moveBy(Vector const& desired_displacement);
  Position const& position() const;
  Rectangle const& rectangle() const;
  size_t width() const;
  size_t x() const;
  size_t y() const;

 private:
  Navigator const& navigator_;
  PositionedRectangle positioned_rectangle_;
};

#endif
