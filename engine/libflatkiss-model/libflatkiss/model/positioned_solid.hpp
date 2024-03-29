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

#ifndef LIBFLATKISS_MODEL_POSITIONED_SOLID_HPP_INCLUDED
#define LIBFLATKISS_MODEL_POSITIONED_SOLID_HPP_INCLUDED

#include <libflatkiss/model/position.hpp>
#include <libflatkiss/model/solid.hpp>
#include <vector>

class PositionedSolid {
 public:
  PositionedSolid(Position const& position, Solid const& solid);
  /**
   * @brief Return the bounding box of the solid moved by the position of the
   * solid.
   *
   * @return PositionedRectangle The bounding box.
   */
  PositionedRectangle absoluteBoundingBox() const;
  /**
   * @brief Return the bounding box in the solid.
   *
   * Note that the position is the
   * smallest position of all the shapes in the solid.
   *
   * @return PositionedRectangle The bounding box.
   */
  PositionedRectangle boundingBox() const;
  /**
   * @brief Returns a new positioned solid moved by the provided displacement.
   *
   * @param vector The displacement to apply.
   * @return PositionedSolid A new positioned solid moved by the vector.
   */
  PositionedSolid operator+(Vector const& vector) const;
  Position const& position() const;
  void position(Position&& new_position);
  std::vector<PositionedEllipse> const& positionedEllipses() const;
  std::vector<PositionedRectangle> const& positionedRectangles() const;
  Solid const& solid() const;
  int64_t x() const;
  int64_t y() const;

 private:
  Position position_;
  // The positioned ellipses are those of the solid, moved by the position.
  std::vector<PositionedEllipse> positioned_ellipses_;
  // The positioned rectangles are those of the solid, moved by the position.
  std::vector<PositionedRectangle> positioned_rectangles_;
  Solid const solid_;

  static std::vector<PositionedEllipse> create_positioned_ellipses(
      Position const& position, Solid const& solid);
  static std::vector<PositionedRectangle> create_positioned_rectangles(
      Position const& position, Solid const& solid);
};

#endif
