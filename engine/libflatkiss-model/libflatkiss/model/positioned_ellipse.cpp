/*
 * Copyright (C) 2021-2022 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Refer to 'COPYING.txt' for the full notice.
 */

#include <libflatkiss/model/positioned_ellipse.hpp>
#include <utility>

using std::move;

PositionedEllipse::PositionedEllipse(Position const& position,
                                     Ellipse const& ellipse)
    : position_{position}, ellipse_{ellipse} {}

PositionedEllipse::PositionedEllipse(PositionedEllipse const& other)
    : position_{other.position_}, ellipse_{other.ellipse_} {}

PositionedEllipse::PositionedEllipse(PositionedEllipse&& other)
    : position_{move(other.position_)}, ellipse_{move(other.ellipse_)} {}

PositionedEllipse::~PositionedEllipse() {}

Ellipse const& PositionedEllipse::ellipse() const { return ellipse_; }

Position const& PositionedEllipse::position() const { return position_; }

int64_t PositionedEllipse::radiusX() const { return ellipse_.radiusX(); }

int64_t PositionedEllipse::radiusY() const { return ellipse_.radiusY(); }

int64_t PositionedEllipse::x() const { return position_.x(); }

int64_t PositionedEllipse::y() const { return position_.y(); }
