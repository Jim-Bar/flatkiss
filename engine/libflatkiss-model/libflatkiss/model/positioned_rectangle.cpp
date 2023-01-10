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

#include <libflatkiss/model/positioned_ellipse.hpp>
#include <libflatkiss/model/positioned_rectangle.hpp>
#include <utility>

using std::move;

PositionedRectangle::PositionedRectangle(Position const& position,
                                         Rectangle const& rectangle)
    : position_{position}, rectangle_{rectangle} {}

PositionedRectangle::PositionedRectangle(PositionedRectangle const& other)
    : position_{other.position()}, rectangle_{other.rectangle()} {}

PositionedRectangle::PositionedRectangle::PositionedRectangle(
    PositionedRectangle&& other)
    : position_{move(other.position_)}, rectangle_{move(other.rectangle_)} {}

PositionedRectangle::~PositionedRectangle() {}

int64_t PositionedRectangle::height() const { return rectangle_.height(); }

PositionedRectangle PositionedRectangle::operator+(Vector const& vector) const {
  return PositionedRectangle{position() + vector, rectangle()};
}

Position const& PositionedRectangle::position() const { return position_; }

void PositionedRectangle::position(Position&& new_position) {
  position_ = move(new_position);
}

Rectangle const& PositionedRectangle::rectangle() const { return rectangle_; }

int64_t PositionedRectangle::width() const { return rectangle_.width(); }

int64_t PositionedRectangle::x() const { return position_.x(); }

void PositionedRectangle::x(int64_t newX) { position_.x(newX); }

int64_t PositionedRectangle::y() const { return position_.y(); }

void PositionedRectangle::y(int64_t newY) { position_.y(newY); }
