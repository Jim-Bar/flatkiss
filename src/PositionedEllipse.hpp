#ifndef POSITIONED_ELLIPSE_HPP_INCLUDED
#define POSITIONED_ELLIPSE_HPP_INCLUDED

#include "Ellipse.hpp"
#include "Position.hpp"

class PositionedEllipse {
public:
    PositionedEllipse(Position const& Position, Ellipse const& Ellipse);
    bool contains(Position const& Position) const;
    Ellipse const& ellipse() const;
    Position const& position() const;
    size_t radiusX() const;
    size_t radiusY() const;
    size_t x() const;
    size_t y() const;

private:
    // Using abreviations because G++ will not let that compile if using the same name as the type.
    Ellipse const Ell;
    Position const Pos;
};

#endif
