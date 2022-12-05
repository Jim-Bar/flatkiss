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

#ifndef COLLIDER_HPP_INCLUDED
#define COLLIDER_HPP_INCLUDED

#include "positioned_ellipse.hpp"
#include "positioned_rectangle.hpp"
#include "positioned_solid.hpp"

/**
 * @brief Collides things.
 */
class Collider {
 public:
  static bool collide(PositionedEllipse const& ellipse1,
                      PositionedEllipse const& ellipse2);
  static bool collide(PositionedEllipse const& ellipse,
                      PositionedRectangle const& rectangle);
  static bool collide(PositionedRectangle const& rectangle,
                      PositionedEllipse const& ellipse);
  static bool collide(PositionedRectangle const& rectangle1,
                      PositionedRectangle const& rectangle2);
  static bool collide(PositionedSolid const& solid1,
                      PositionedSolid const& solid2);

 private:
  static bool collide(PositionedEllipse const& ellipse,
                      Position const& position);
};

#endif
