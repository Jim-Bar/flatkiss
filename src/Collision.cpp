#include "Collision.hpp"

#include <fstream>
#include <set>
#include <vector>

using std::move;
using std::set;
using std::unordered_map;
using std::vector;

Collision::Collision(std::vector<PositionedEllipse> const PositionedEllipses,
                     vector<PositionedRectangle> const PositionedRectangles)
    : PositionedEllipses(move(PositionedEllipses)),
      PositionedRectangles(move(PositionedRectangles)) {}

bool Collision::collidesWith(PositionedRectangle const& PositionedRect,
                             Position const WhenAtPosition) const {
  for (auto PositionedEllipse : PositionedEllipses) {
    if (PositionedRect.intersectsWith(WhenAtPosition + PositionedEllipse)) {
      return true;
    }
  }

  for (auto PositionedRectangle : PositionedRectangles) {
    if (PositionedRect.intersectsWith(WhenAtPosition + PositionedRectangle)) {
      return true;
    }
  }

  return false;
}

unordered_map<uint16_t, Collision const> CollisionLoader::load(
    std::string const& FilePath) {
  // First read all the positioned rectangles for all the tile indices.
  std::ifstream Stream;
  set<uint16_t> AllTilesIndices;
  unordered_map<uint16_t, vector<PositionedEllipse>> EllipsesPerTileIndex;
  unordered_map<uint16_t, vector<PositionedRectangle>> RectanglesPerTileIndex;
  Stream.open(FilePath, std::ios::in | std::ios::binary);
  if (Stream.is_open()) {
    while (Stream.peek() != std::istream::traits_type::eof()) {
      uint16_t TileIndex{0};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      Stream.read(reinterpret_cast<char*>(&TileIndex),
                  2);  // Two bytes per tile index.
      AllTilesIndices.insert(TileIndex);
      uint8_t CollisionType{static_cast<uint8_t>(Stream.get())};
      Position Position{static_cast<uint8_t>(Stream.get()),
                        static_cast<uint8_t>(Stream.get())};
      switch (CollisionType) {
        case 0:  // The collision is a positioned rectangle.
          if (!RectanglesPerTileIndex.contains(TileIndex)) {
            RectanglesPerTileIndex.emplace(TileIndex,
                                           vector<PositionedRectangle>{});
          }
          RectanglesPerTileIndex[TileIndex].emplace_back(
              Position, Rectangle{static_cast<uint8_t>(Stream.get()),
                                  static_cast<uint8_t>(Stream.get())});
          break;
        case 1:  // The collision is a positioned ellipse.
          if (!EllipsesPerTileIndex.contains(TileIndex)) {
            EllipsesPerTileIndex.emplace(TileIndex,
                                         vector<PositionedEllipse>{});
          }
          EllipsesPerTileIndex[TileIndex].emplace_back(
              Position, Ellipse{static_cast<uint8_t>(Stream.get()),
                                static_cast<uint8_t>(Stream.get())});
          break;
      }
    }
    Stream.close();
  }  // FIXME: Raise exception.

  /* Secondly, once all the positioned rectangles / ellipses are known, the
   * collisions can be built. */
  unordered_map<uint16_t, Collision const> CollisionsPerTileIndex;
  for (auto TileIndex : AllTilesIndices) {
    CollisionsPerTileIndex.emplace(
        std::piecewise_construct, std::forward_as_tuple(TileIndex),
        std::forward_as_tuple(move(EllipsesPerTileIndex[TileIndex]),
                              move(RectanglesPerTileIndex[TileIndex])));
  }

  return CollisionsPerTileIndex;
}
