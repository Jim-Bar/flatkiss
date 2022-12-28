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

#include <libflatkiss/model/positioned_solid.hpp>
#include <utility>

using std::move;
using std::vector;

PositionedSolid::PositionedSolid(Position const& position, Solid const& solid)
    : position_{position},
      solid_{solid},
      positioned_ellipses_{create_positioned_ellipses(position, solid)},
      positioned_rectangles_{create_positioned_rectangles(position, solid)} {}

PositionedRectangle PositionedSolid::absoluteBoundingBox() const {
  return position_ + solid_.boundingBox();
}

PositionedRectangle PositionedSolid::boundingBox() const {
  return solid_.boundingBox();
}

vector<PositionedEllipse> PositionedSolid::create_positioned_ellipses(
    Position const& position, Solid const& solid) {
  vector<PositionedEllipse> positioned_ellipses{};
  for (auto pos_ellipse : solid.positionedEllipses()) {
    positioned_ellipses.emplace_back(pos_ellipse.position() + position,
                                     pos_ellipse.ellipse());
  }

  return positioned_ellipses;
}

vector<PositionedRectangle> PositionedSolid::create_positioned_rectangles(
    Position const& position, Solid const& solid) {
  vector<PositionedRectangle> positioned_rectangles{};
  for (auto pos_rectangle : solid.positionedRectangles()) {
    positioned_rectangles.emplace_back(pos_rectangle.position() + position,
                                       pos_rectangle.rectangle());
  }

  return positioned_rectangles;
}

PositionedSolid PositionedSolid::operator+(Vector const& vector) const {
  return PositionedSolid{position() + vector, solid_};
}

Position const& PositionedSolid::position() const { return position_; }

void PositionedSolid::position(Position&& new_position) {
  position_ = move(new_position);
}

vector<PositionedEllipse> const& PositionedSolid::positionedEllipses() const {
  return positioned_ellipses_;
}

vector<PositionedRectangle> const& PositionedSolid::positionedRectangles()
    const {
  return positioned_rectangles_;
}

Solid const& PositionedSolid::solid() const { return solid_; }

int64_t PositionedSolid::x() const { return position_.x(); }

int64_t PositionedSolid::y() const { return position_.y(); }
