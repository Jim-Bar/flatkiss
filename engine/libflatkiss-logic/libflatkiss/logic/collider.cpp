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
#include <libflatkiss/logic/collider.hpp>

using std::ceil;
using std::fabsl;
using std::sqrtl;

int64_t square(int64_t value) { return value * value; }
long double square(long double value) { return value * value; }
long double vectorNorm(long double vX, long double vY) {
  return sqrtl(square(vX) + square(vY));
}

// For a circle at coordinates (a, b) with radius `r`.
// (x - a) ^ 2 + (y - b) ^ 2 = r ^ 2.
struct FloatCircle {
  long double a;
  long double b;
  long double r;
};

// For an ellipse centered at the origin with width 2a and height 2b and
// assuming a >= b:
// (x ^ 2) / (a ^ 2) + (y ^ 2) / (b ^ 2) = 1.
// c = sqrt(a ^ 2 - b ^ 2)
struct FloatEllipse {
  long double a;
  long double b;
  long double c;
};

bool Collider::collide(PositionedEllipse const& ellipse1,
                       PositionedEllipse const& ellipse2) {
  // TODO: https://stackoverflow.com/a/2945439

  // Deformation to create a circle from the second ellipse.
  long double dX{static_cast<long double>(ellipse2.radiusY())};
  long double dY{static_cast<long double>(ellipse2.radiusX())};

  // Translation to center the first ellipse at the origin.
  long double tX{static_cast<long double>(-ellipse1.x()) * dX};
  long double tY{static_cast<long double>(-ellipse1.y()) * dY};

  // Second ellipse to circle with floating point precision.
  FloatCircle circle{};
  circle.a = static_cast<long double>(ellipse2.x()) * dX + tX;
  circle.b = static_cast<long double>(ellipse2.y()) * dY + tY;
  circle.r = static_cast<long double>(ellipse2.radiusX()) * dX;

  // First ellipse to origin with floating point precision.
  FloatEllipse ellipse{};
  ellipse.a = static_cast<long double>(ellipse1.radiusX()) * dX;
  ellipse.b = static_cast<long double>(ellipse1.radiusY()) * dY;
  ellipse.c = sqrtl(fabsl(square(ellipse.a) - square(ellipse.b)));

  // Swap axes so that the major axis of the ellipse is horizontal.
  if (ellipse.a < ellipse.b) {
    // Swap axes of the ellipse.
    ellipse.a += ellipse.b;
    ellipse.b = ellipse.a - ellipse.b;
    ellipse.a -= ellipse.b;
    // Swap coordinates of the circle.
    circle.a += circle.b;
    circle.b = circle.a - circle.b;
    circle.a -= circle.b;
  }

  /* Now the ellipse is centered at the origin, with horizontal radius `a` and
   * vertical radius `b` and the distance to the foci `c`. The major axis
   * coincides with the horizontal axis. */

  // Generate points to check on the major axis of the ellipse.
  int64_t num_points{static_cast<int64_t>(ceill(2 * ellipse.a))};
  long double step{2 * ellipse.a / static_cast<long double>(num_points)};
  for (int64_t i{0}; i < num_points; i++) {
    long double x{-ellipse.a + static_cast<long double>(i) * step};
    long double y{0};

    /* Point of intersection of the circle with the ray cast from the center of
     * the circle and passing by (x, y). */
    long double vX{x - circle.a};
    long double vY{y - circle.b};
    long double norm{vectorNorm(vX, vY)};
    // Transform the vector so that its norm equals the radius of the circle.
    vX *= circle.r / norm;
    vY *= circle.r / norm;

    // Point on the circle.
    x = circle.a + vX;
    y = circle.b + vY;

    long double f1X{-ellipse.c};
    long double f1Y{0};
    long double f2X{ellipse.c};
    long double f2Y{0};
    if (vectorNorm(x - f1X, y - f1Y) + vectorNorm(x - f2X, y - f2Y) <= 2 * ellipse.a) {
      return true;
    }
  }

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
   * Okay, reading this could sound complex, but it is actually not that
   * complex. Just grab a pen a draw ellipses and rectangles. */

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
