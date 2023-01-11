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

#include <cmath>
#include <libflatkiss/model/positioned_ellipse.hpp>
#include <utility>

using std::abs;
using std::llround;
using std::make_pair;
using std::move;
using std::pair;
using std::pow;
using std::sqrt;

PositionedEllipse::PositionedEllipse(Position const& position,
                                     Ellipse const& ellipse)
    : position_{position}, ellipse_{ellipse} {}

PositionedEllipse::PositionedEllipse(PositionedEllipse const& other)
    : position_{other.position_}, ellipse_{other.ellipse_} {}

PositionedEllipse::PositionedEllipse(PositionedEllipse&& other)
    : position_{move(other.position_)}, ellipse_{move(other.ellipse_)} {}

PositionedEllipse::~PositionedEllipse() {}

Ellipse const& PositionedEllipse::ellipse() const { return ellipse_; }

pair<Position, Position> PositionedEllipse::focalPoints() const {
  /* The linear eccentricity c is the distance from the ellipse's center to the
   * focal points. So if radiusX > radiusY, the focal points are (c, 0) and (-c,
   * 0). Otherwise (0, c) and (0, -c). Then they just need to be translated by
   * the actual position of the ellipse's center. */

  // 1. Compute linear eccentricity.
  long double square_radius_x{pow(radiusX(), 2)};
  long double square_radius_y{pow(radiusY(), 2)};
  int64_t c{llround(sqrt(abs(square_radius_x - square_radius_y)))};

  // 2. Find the position of the focal points in the ellipse.
  auto focal_points{make_pair(Position{0, 0}, Position{0, 0})};
  if (radiusX() > radiusY()) {
    focal_points.first = Position{c, 0};
    focal_points.second = Position{-c, 0};
  } else {
    focal_points.first = Position{0, c};
    focal_points.second = Position{0, -c};
  }

  // 3. Translate to the actual coordinates.
  focal_points.first += position();
  focal_points.second += position();

  return focal_points;
}

Position const& PositionedEllipse::position() const { return position_; }

int64_t PositionedEllipse::radiusX() const { return ellipse_.radiusX(); }

int64_t PositionedEllipse::radiusY() const { return ellipse_.radiusY(); }

int64_t PositionedEllipse::x() const { return position_.x(); }

int64_t PositionedEllipse::y() const { return position_.y(); }
