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

#include <algorithm>
#include <libflatkiss/model/solid.hpp>
#include <limits>
#include <utility>
#include <vector>

using std::max;
using std::min;
using std::move;
using std::numeric_limits;
using std::vector;

Solid::Solid(vector<PositionedEllipse> positioned_ellipses,
             vector<PositionedRectangle> positioned_rectangles)
    : bounding_box_{Solid::computeBoundingBox(positioned_ellipses,
                                              positioned_rectangles)},
      positioned_ellipses_{move(positioned_ellipses)},
      positioned_rectangles_{move(positioned_rectangles)} {}

PositionedRectangle const& Solid::boundingBox() const { return bounding_box_; }

PositionedRectangle Solid::computeBoundingBox(
    std::vector<PositionedEllipse> positioned_ellipses,
    std::vector<PositionedRectangle> positioned_rectangles) {
  // This assumes there is at least one item.
  int64_t max_x{numeric_limits<int64_t>::min()};
  int64_t max_y{numeric_limits<int64_t>::min()};
  int64_t min_x{numeric_limits<int64_t>::max()};
  int64_t min_y{numeric_limits<int64_t>::max()};

  for (auto const& positioned_ellipse : positioned_ellipses) {
    max_x = max(max_x, positioned_ellipse.x() + positioned_ellipse.radiusX());
    max_y = max(max_y, positioned_ellipse.y() + positioned_ellipse.radiusY());
    min_x = min(min_x, positioned_ellipse.x() - positioned_ellipse.radiusX());
    min_y = min(min_y, positioned_ellipse.y() - positioned_ellipse.radiusY());
  }

  for (auto const& positioned_rectangle : positioned_rectangles) {
    max_x = max(max_x, positioned_rectangle.x() + positioned_rectangle.width());
    max_y =
        max(max_y, positioned_rectangle.y() + positioned_rectangle.height());
    min_x = min(min_x, positioned_rectangle.x());
    min_y = min(min_y, positioned_rectangle.y());
  }

  return PositionedRectangle{Position{min_x, min_y},
                             Rectangle{max_x - min_x, max_y - min_y}};
}

vector<PositionedEllipse> const& Solid::positionedEllipses() const {
  return positioned_ellipses_;
}

vector<PositionedRectangle> const& Solid::positionedRectangles() const {
  return positioned_rectangles_;
}
