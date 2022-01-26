#ifndef COLLISION_HPP_INCLUDED
#define COLLISION_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>

#include "PositionedRectangle.hpp"

/**
 * @brief An area made up of several rectangles and which cannot be crossed.
 */
class Collision {
public:
    Collision(std::vector<PositionedRectangle> const PositionedRectangles, size_t RectanglesCount);
    bool collidesWith(PositionedRectangle const& PositionedRectangle, Position const WhenAtPosition) const;

private:
    std::vector<PositionedRectangle> const PositionedRectangles;
    size_t const RectanglesCount;
};

class CollisionLoader {
public:
    static std::unordered_map<uint16_t, Collision const> load(std::string const& FilePath);
};

#endif
