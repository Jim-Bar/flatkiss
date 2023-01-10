/*
 * Copyright (C) 2021-2023 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
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

#include <libflatkiss/model/position.hpp>
#include <libflatkiss/model/positioned_ellipse.hpp>
#include <libflatkiss/model/positioned_rectangle.hpp>
#include <libflatkiss/model/rectangle.hpp>
#include <utility>

using std::move;

Position::Position(int64_t X, int64_t Y) : x_{X}, y_{Y} {}

Position::Position(Position const& other) : x_{other.x()}, y_{other.y()} {}

Position::Position(Position&& other) : x_{move(other.x_)}, y_{move(other.y_)} {}

Position::~Position() {}

Position& Position::operator=(Position&& other) {
  x_ = move(other.x_);
  y_ = move(other.y_);
  return *this;
}

bool Position::operator!=(Position const& other) const {
  return !(*this == other);
}

Position Position::operator+(Position const& other) const {
  return Position{x() + other.x(), y() + other.y()};
}

PositionedEllipse Position::operator+(PositionedEllipse const& ellipse) const {
  return PositionedEllipse{*this + ellipse.position(), ellipse.ellipse()};
}

PositionedRectangle Position::operator+(
    PositionedRectangle const& rectangle) const {
  return PositionedRectangle{*this + rectangle.position(),
                             rectangle.rectangle()};
}

Position Position::operator+(Vector const& vector) const {
  return Position{x() + vector.dx(), y() + vector.dy()};
}

Vector Position::operator-(Position const& other) const {
  return Vector{x() - other.x(), y() - other.y()};
}

bool Position::operator==(Position const& other) const {
  return x() == other.x() && y() == other.y();
}

int64_t Position::x() const { return x_; }

void Position::x(int64_t newX) { x_ = newX; }

int64_t Position::y() const { return y_; }

void Position::y(int64_t newY) { y_ = newY; }
