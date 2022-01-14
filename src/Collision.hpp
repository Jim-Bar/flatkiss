#ifndef COLLISION_HPP_INCLUDED
#define COLLISION_HPP_INCLUDED

#include <string>
#include <unordered_map>

/**
 * @brief For now just a rectangle delimitating a zone which cannot be crossed.
 */
class Collision {
public:
    Collision(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
    uint8_t height() const;
    uint8_t x() const;
    uint8_t y() const;
    uint8_t width() const;

private:
    uint8_t Height;
    uint8_t X;
    uint8_t Y;
    uint8_t Width;
};

class CollisionLoader {
public:
    static std::unordered_map<uint16_t, Collision const> load(std::string const& FilePath);
};

#endif
