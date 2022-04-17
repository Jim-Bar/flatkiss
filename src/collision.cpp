#include "collision.hpp"

#include <algorithm>
#include <utility>
#include <vector>

using std::move;
using std::vector;
using std::ranges::any_of;

Collision::Collision(vector<PositionedEllipse> positioned_ellipses,
                     vector<PositionedRectangle> positioned_rectangles)
    : positioned_ellipses_{move(positioned_ellipses)},
      positioned_rectangles_{move(positioned_rectangles)} {}

bool Collision::collidesWith(PositionedRectangle const& positioned_rectangle,
                             Position const& when_at_position) const {
  for (auto ellipse : positioned_ellipses_) {
    if (positioned_rectangle.intersectsWith(when_at_position + ellipse)) {
      return true;
    }
  }

  return any_of(positioned_rectangles_,
                [&](PositionedRectangle const& rectangle) {
                  return positioned_rectangle.intersectsWith(when_at_position +
                                                             rectangle);
                });
}
