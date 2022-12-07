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

#ifndef LIBFLATKISS_MODEL_POSITIONED_RECTANGLE_HPP_INCLUDED
#define LIBFLATKISS_MODEL_POSITIONED_RECTANGLE_HPP_INCLUDED

#include <libflatkiss/model/position.hpp>
#include <libflatkiss/model/rectangle.hpp>
#include <libflatkiss/model/vector.hpp>

/**
 * @brief A rectangle with a position.
 */
class PositionedRectangle {
 public:
  PositionedRectangle(Position const& position, Rectangle const& rectangle);
  PositionedRectangle(PositionedRectangle const& other);
  PositionedRectangle(PositionedRectangle&& other);
  ~PositionedRectangle();
  PositionedRectangle& operator=(PositionedRectangle const& other) = delete;
  PositionedRectangle& operator=(PositionedRectangle&& other) = delete;
  int64_t height() const;
  /**
   * @brief Returns a new positioned rectangle moved by the provided
   * displacement.
   *
   * @param vector The displacement to apply.
   * @return PositionedRectangle A new positioned rectangle moved by the vector.
   */
  PositionedRectangle operator+(Vector const& vector) const;
  Position const& position() const;
  void position(Position&& new_position);
  Rectangle const& rectangle() const;
  int64_t width() const;
  int64_t x() const;
  void x(int64_t newX);
  int64_t y() const;
  void y(int64_t newY);

 private:
  Position position_;
  Rectangle const rectangle_;
};

#endif
