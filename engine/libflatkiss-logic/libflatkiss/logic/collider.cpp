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

using std::abs;
using std::sqrtl;

int64_t square(int64_t value) { return value * value; }
long double square(long double value) { return value * value; }

// (x - a) ^ 2 + (y - b) ^ 2 = r ^ 2.
struct Circle {
  long double a;
  long double b;
  long double r;
};

struct Ellipse {
  long double f1X;
  long double f1Y;
  long double f2X;
  long double f2Y;
  long double cX;
  long double cY;
  long double rX;
  long double rY;
};

// y = cx + d.
struct StraightLine {
  long double c;
  long double d;
};

struct Segment {
  StraightLine const& line;
  long double lowerBoundX;
  long double upperBoundX;
};

bool intersect(Circle const& circle, Segment const& segment) {
  long double const& a{circle.a};
  long double const& b{circle.b};
  long double const& r{circle.r};
  long double const& c{segment.line.c};
  long double const& d{segment.line.d};

  long double delta{-4 * square(c) * square(d) + 8 * a * c * d +
                    8 * square(c) * b * d - 8 * square(a) - 8 * a * c * b -
                    4 * square(c) * square(b) - 4 * square(b) - 4 * square(d) +
                    8 * d * b - 4 * square(r)};

  if (delta < 0) {
    return false;
  }

  long double x1{(2 * a + 2 * c * b - 2 * c * d - sqrtl(delta)) / 2};
  long double y1{c * x1 + d};
  long double x2{(2 * a + 2 * c * b - 2 * c * d + sqrtl(delta)) / 2};
  long double y2{c * x2 + d};

  return (segment.lowerBoundX <= x1 && x1 <= segment.upperBoundX) ||
         (segment.lowerBoundX <= x2 && x2 <= segment.upperBoundX);
}

bool Collider::collide(PositionedEllipse const& ellipse1,
                       PositionedEllipse const& ellipse2) {
  // TODO: https://stackoverflow.com/a/2945439
  // Will add the test AE + BE <= e with E center of the ellipse.

  // Deformation to create a circle from the second ellipse.
  long double dX{static_cast<long double>(ellipse2.radiusY())};
  long double dY{static_cast<long double>(ellipse2.radiusX())};

  // Ellipse 2 to circle with floating point precision.
  Circle circle{};
  circle.a = static_cast<long double>(ellipse2.x() * dX);
  circle.b = static_cast<long double>(ellipse2.y() * dY);
  circle.r = static_cast<long double>(ellipse2.radiusX() * dX);

  // Ellipse 1 to floating point precision.
  Ellipse ellipse{};
  auto focalPoints{ellipse1.focalPoints()};
  ellipse.f1X = static_cast<long double>(focalPoints.first.x()) * dX;
  ellipse.f1Y = static_cast<long double>(focalPoints.first.y()) * dY;
  ellipse.f2X = static_cast<long double>(focalPoints.second.x()) * dX;
  ellipse.f2Y = static_cast<long double>(focalPoints.second.y()) * dY;
  ellipse.cX = static_cast<long double>(ellipse1.x()) * dX;
  ellipse.cY = static_cast<long double>(ellipse1.y()) * dY;
  ellipse.rX = static_cast<long double>(ellipse1.radiusX()) * dX;
  ellipse.rY = static_cast<long double>(ellipse1.radiusY()) * dY;

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
