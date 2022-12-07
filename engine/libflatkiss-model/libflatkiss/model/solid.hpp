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

#ifndef LIBFLATKISS_MODEL_SOLID_HPP_INCLUDED
#define LIBFLATKISS_MODEL_SOLID_HPP_INCLUDED

#include <libflatkiss/model/positioned_ellipse.hpp>
#include <libflatkiss/model/positioned_rectangle.hpp>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief An area made up of several shapes and which cannot be crossed.
 */
class Solid {
 public:
  Solid(std::vector<PositionedEllipse> positioned_ellipses,
        std::vector<PositionedRectangle> positioned_rectangles);
  /**
   * @brief Return the minimal rectangle containing all the shapes and
   * positioned inside the solid at the location (x, y) defined by the smallest
   * x-position (respectively y-position) among all the shapes.
   *
   * @return PositionedRectangle const& The bounding box.
   */
  PositionedRectangle const& boundingBox() const;
  // TODO: Use an abstract class PositionedShape?
  std::vector<PositionedEllipse> const& positionedEllipses() const;
  std::vector<PositionedRectangle> const& positionedRectangles() const;

 private:
  PositionedRectangle const bounding_box_;
  std::vector<PositionedEllipse> const positioned_ellipses_;
  std::vector<PositionedRectangle> const positioned_rectangles_;

  static PositionedRectangle computeBoundingBox(
      std::vector<PositionedEllipse> positioned_ellipses,
      std::vector<PositionedRectangle> positioned_rectangles);
};

/**
 * @brief Helper class for loading the solids from a file.
 */
class SolidLoader {
 public:
  static std::unordered_map<int64_t, Solid const> load(
      std::string const& file_path);

 private:
  static Solid loadSolid(int64_t solid_size, std::ifstream& solids_stream);
};

#endif