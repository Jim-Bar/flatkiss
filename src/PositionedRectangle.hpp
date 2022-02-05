#ifndef POSITIONED_RECTANGLE_HPP_INCLUDED
#define POSITIONED_RECTANGLE_HPP_INCLUDED

#include "Position.hpp"
#include "Rectangle.hpp"
#include "Vector.hpp"

/**
 * @brief A rectangle with a position.
 *
 * Provides useful routines such as intersection helpers.
 */
class PositionedRectangle {
public:
    PositionedRectangle(Position const Position, Rectangle const Rectangle);
    size_t height() const;
    bool intersectsWith(PositionedRectangle const& Other) const;
    /**
     * @brief Returns a new positioned rectangle moved by the provided displacement.
     *
     * @param Vector The displacement to apply.
     * @return PositionedRectangle A new positioned rectangle moved by the vector.
     */
    PositionedRectangle operator+(Vector const& Vector) const;
    Position const& position() const;
    Rectangle const& rectangle() const;
    size_t width() const;
    size_t x() const;
    size_t y() const;

private:
    // Using abreviations because G++ will not let that compile if using the same name as the type.
    Position const Pos;
    Rectangle const Rect;
};

#endif
