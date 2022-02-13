#ifndef COLLISION_HPP_INCLUDED
#define COLLISION_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>

#include "positioned_ellipse.hpp"
#include "positioned_rectangle.hpp"

/**
 * @brief An area made up of several rectangles and which cannot be crossed.
 */
class Collision {
 public:
  Collision(std::vector<PositionedEllipse> const positioned_ellipses,
            std::vector<PositionedRectangle> const positioned_rectangles);
  bool collidesWith(PositionedRectangle const& positioned_rectangle,
                    Position const& when_at_position) const;

 private:
  std::vector<PositionedEllipse> const positioned_ellipses_;
  std::vector<PositionedRectangle> const positioned_rectangles_;
};

class CollisionLoader {
 public:
  static std::unordered_map<uint16_t, Collision const> load(
      std::string const& file_path);
};

#endif
