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

#include "collider.hpp"

#include <cmath>

using std::abs;

int64_t square(int64_t value) { return value * value; }

bool Collider::collide(PositionedEllipse const& ellipse1,
                       PositionedEllipse const& ellipse2) {
  // TODO: do.
  return false;
}

bool Collider::collide(PositionedEllipse const& ellipse,
                       PositionedRectangle const& rectangle) {
  return collide(rectangle, ellipse);
}

bool Collider::collide(PositionedRectangle const& r,
                       PositionedEllipse const& e) {
  /* Checking that a rectangle (or even line for that matter) intersects with an
   * ellipse is no easy task. However this is a simpler case because both the
   * rectangle and ellipse are axis aligned (i.e. not rotated). So the check
   * boils down to:
   * - test whether the ellipse contains one of the four points of the rectangle
   * - test for each vertex and co-vertex of the ellipse whether it is on the
   * other side of the closest side of the rectangle, and within the bounds of
   * the two adjacent sides of the rectangle
   *
   * Note for the second test that checking whether each (co-)vertex is
   * contained in the rectangle is not enough: if the rectangle moved too fast
   * (or is too thin), the (co-)vertex could end up beyond the rectangle. The
   * second test is needed when the rectangle is larger than the ellipse.
   *
   * Okay, reading this could sounds complex, but it is actually not that
   * complex. Just take a pen a draw ellipses and rectangles. */

  // First test.
  if (collide(e, r.position()) ||
      collide(e, r.position() + Vector{r.width(), 0}) ||
      collide(e, r.position() + Vector(0, r.height())) ||
      collide(e, r.position() + Vector{r.width(), r.height()})) {
    return true;
  }

  // Second test, part one.
  if (e.x() >= r.x() && e.x() <= r.x() + r.width()) {
    if (e.y() + e.radiusY() >= r.y() || e.y() >= r.y() + e.radiusY()) {
      return true;
    }
  }

  // End of the second test.
  if (e.y() >= r.y() && e.y() <= r.y() + r.height()) {
    if (e.x() + e.radiusX() >= r.x() || e.x() >= r.x() + e.radiusX()) {
      return true;
    }
  }

  return false;
}

bool Collider::collide(PositionedRectangle const& rectangle1,
                       PositionedRectangle const& rectangle2) {
  if (rectangle1.x() + rectangle1.width() <= rectangle2.x()) {
    return false;
  }

  if (rectangle1.x() > rectangle2.x() + rectangle2.width()) {
    return false;
  }

  if (rectangle1.y() + rectangle1.height() <= rectangle2.y()) {
    return false;
  }

  if (rectangle1.y() > rectangle2.y() + rectangle2.height()) {
    return false;
  }

  return true;
}

bool Collider::collide(PositionedEllipse const& e, Position const& p) {
  // https://math.stackexchange.com/a/76463
  return square(e.radiusY() * abs(p.x() - e.x())) +
             square(e.radiusX() * abs(p.y() - e.y())) <=
         square(e.radiusX() * e.radiusY());
}

bool Collider::collide(PositionedSolid const& solid1,
                       PositionedSolid const& solid2) {
  for (auto const& pos_ell1 : solid1.positionedEllipses()) {
    for (auto const& pos_ell2 : solid2.positionedEllipses()) {
      if (collide(pos_ell1, pos_ell2)) {
        return true;
      }
    }
    for (auto const& pos_rect2 : solid2.positionedRectangles()) {
      if (collide(pos_rect2, pos_ell1)) {
        return true;
      }
    }
  }

  for (auto const& pos_rect1 : solid1.positionedRectangles()) {
    for (auto const& pos_ell2 : solid2.positionedEllipses()) {
      if (collide(pos_rect1, pos_ell2)) {
        return true;
      }
    }
    for (auto const& pos_rect2 : solid2.positionedRectangles()) {
      if (collide(pos_rect1, pos_rect2)) {
        return true;
      }
    }
  }

  return false;
}
