#ifndef COLLISION_HPP_INCLUDED
#define COLLISION_HPP_INCLUDED

#include <memory>
#include <string>
#include <unordered_map>

#include "PositionedRectangle.hpp"

/**
 * @brief For now just a rectangle delimitating a zone which cannot be crossed.
 */
class Collision {
public:
    Collision(std::unique_ptr<PositionedRectangle const[]> PositionedRectangles, size_t RectanglesCount);
    bool collidesWith(PositionedRectangle const& PositionedRectangle, Position const WhenAtPosition) const;

private:
    std::unique_ptr<PositionedRectangle const[]> PositionedRectangles;
    size_t const RectanglesCount;
};

class CollisionLoader {
public:
    static std::unordered_map<uint16_t, Collision const> load(std::string const& FilePath);
};

#endif
