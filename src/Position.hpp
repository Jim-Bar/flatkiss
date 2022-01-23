#ifndef POSITION_HPP_INCLUDED
#define POSITION_HPP_INCLUDED

#include <cstddef>

class PositionedRectangle; // Forward declaration to break the cycle Position / PositionedRectangle.

/**
 * @brief A position in pixels somewhere in the world, viewport or whatever.
 *
 * Note that this is always in pixels. This class must not be used for things like location in the world in tiles.
 */
class Position {
public:
    Position(size_t X, size_t Y);
    /**
     * @brief Returns a new positioned rectangle moved by the position.
     * The position here can be considered as a vector, or a distance.
     *
     * @param PositionedRectangle The positioned rectangle that will be moved.
     * @return PositionedRectangle A new positioned rectangle moved by the current position.
     */
    PositionedRectangle operator+(PositionedRectangle const& PositionedRectangle) const;
    size_t x() const;
    size_t y() const;

private:
    size_t const X;
    size_t const Y;
};

#endif
