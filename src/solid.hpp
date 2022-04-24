#ifndef SOLID_HPP_INCLUDED
#define SOLID_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>

#include "positioned_ellipse.hpp"
#include "positioned_rectangle.hpp"

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
