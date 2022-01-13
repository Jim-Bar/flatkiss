#ifndef COLLISION_HPP_INCLUDED
#define COLLISION_HPP_INCLUDED

#include <string>
#include <unordered_map>

class Collision {
public:
    Collision(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
};

class CollisionLoader {
public:
    static std::unordered_map<uint16_t, Collision const> load(std::string const& FilePath);
};

#endif
