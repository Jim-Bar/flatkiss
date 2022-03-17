#ifndef MOVABLE_POSITION_HPP_INCLUDED
#define MOVABLE_POSITION_HPP_INCLUDED

#include "position.hpp"

/**
 * @brief A Position which can be moved.
 */
class MovablePosition : public Position {
 public:
  MovablePosition(MovablePosition const& other);
  MovablePosition(MovablePosition&& other);
  MovablePosition(Position const& other);
  MovablePosition(Position&& other);
  MovablePosition& operator=(Position const& other);
  MovablePosition& operator=(MovablePosition&& other);
  void setX(size_t x);
  void setY(size_t y);
};

#endif
