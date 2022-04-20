#include "collision.hpp"

#include <algorithm>
#include <limits>
#include <utility>
#include <vector>

using std::max;
using std::min;
using std::move;
using std::numeric_limits;
using std::vector;
using std::ranges::any_of;

Collision::Collision(vector<PositionedEllipse> positioned_ellipses,
                     vector<PositionedRectangle> positioned_rectangles)
    : bounding_box_{Collision::computeBoundingBox(positioned_ellipses,
                                                  positioned_rectangles)},
      positioned_ellipses_{move(positioned_ellipses)},
      positioned_rectangles_{move(positioned_rectangles)} {}

PositionedRectangle const& Collision::boundingBox() const {
  return bounding_box_;
}

PositionedRectangle Collision::computeBoundingBox(
    std::vector<PositionedEllipse> positioned_ellipses,
    std::vector<PositionedRectangle> positioned_rectangles) {
  // This assumes there is at least one item.
  int64_t max_x{numeric_limits<int64_t>::min()};
  int64_t max_y{numeric_limits<int64_t>::min()};
  int64_t min_x{numeric_limits<int64_t>::max()};
  int64_t min_y{numeric_limits<int64_t>::max()};

  for (PositionedEllipse const& positioned_ellipse : positioned_ellipses) {
    max_x = max(max_x, positioned_ellipse.x() + positioned_ellipse.radiusX());
    max_y = max(max_y, positioned_ellipse.y() + positioned_ellipse.radiusY());
    min_x = min(min_x, positioned_ellipse.x() - positioned_ellipse.radiusX());
    min_y = min(min_y, positioned_ellipse.y() - positioned_ellipse.radiusY());
  }

  for (PositionedRectangle const& positioned_rectangle :
       positioned_rectangles) {
    max_x = max(max_x, positioned_rectangle.x() + positioned_rectangle.width());
    max_y =
        max(max_y, positioned_rectangle.y() + positioned_rectangle.height());
    min_x = min(min_x, positioned_rectangle.x());
    min_y = min(min_y, positioned_rectangle.y());
  }

  return PositionedRectangle{Position{min_x, min_y},
                             Rectangle{max_x - min_x, max_y - min_y}};
}

vector<PositionedEllipse> const& Collision::positionedEllipses() const {
  return positioned_ellipses_;
}

vector<PositionedRectangle> const& Collision::positionedRectangles() const {
  return positioned_rectangles_;
}
