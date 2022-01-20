#ifndef POSITION_HPP_INCLUDED
#define POSITION_HPP_INCLUDED

#include <cstddef>

/**
 * @brief A position in pixels somewhere in the world, viewport or whatever.
 *
 * Note that this is always in pixels. This class must not be used for things like location in the world in tiles.
 */
class Position {
public:
    Position(size_t X, size_t Y);
    size_t x() const;
    size_t y() const;

private:
    size_t const X;
    size_t const Y;
};

#endif
