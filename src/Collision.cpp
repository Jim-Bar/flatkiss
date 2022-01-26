#include <fstream>
#include <vector>

#include "Collision.hpp"

using std::move;
using std::unordered_map;
using std::vector;

Collision::Collision(vector<PositionedRectangle> const PositionedRectangles, size_t RectanglesCount) : PositionedRectangles(move(PositionedRectangles)), RectanglesCount(RectanglesCount) {

}

bool Collision::collidesWith(PositionedRectangle const& PositionedRect, Position const WhenAtPosition) const {
    for (auto PositionedRectangle : PositionedRectangles) {
        if (PositionedRect.intersectsWith(WhenAtPosition + PositionedRectangle)) {
            return true;
        }
    }

    return false;
}

unordered_map<uint16_t, Collision const> CollisionLoader::load(std::string const& FilePath) {
    // First read all the positioned rectangles for all the tile indices.
    std::ifstream Stream;
    unordered_map<uint16_t, vector<PositionedRectangle>> VectorsPerTileIndex;
    Stream.open(FilePath, std::ios::in | std::ios::binary);
    if (Stream.is_open()) {
        while (Stream.peek() != std::istream::traits_type::eof()) {
            uint16_t TileIndex{0};
            Stream.read(reinterpret_cast<char*>(&TileIndex), 2); // Two bytes per tile index.
            Position Position{static_cast<uint8_t>(Stream.get()), static_cast<uint8_t>(Stream.get())};
            Rectangle Rectangle{static_cast<uint8_t>(Stream.get()), static_cast<uint8_t>(Stream.get())};
            if (VectorsPerTileIndex.find(TileIndex) == VectorsPerTileIndex.end()) { // FIXME: Use contains()
                VectorsPerTileIndex.emplace(TileIndex, vector<PositionedRectangle>{});
            }
            VectorsPerTileIndex[TileIndex].emplace_back(Position, Rectangle);
        }
        Stream.close();
    } // FIXME: Raise exception.

    // Secondly, once all the positioned rectangles are known, the collisions can be built.
    unordered_map<uint16_t, Collision const> CollisionsPerTileIndex;
    for (auto Iterator : VectorsPerTileIndex) {
        CollisionsPerTileIndex.emplace(std::piecewise_construct, std::forward_as_tuple(Iterator.first), std::forward_as_tuple(move(Iterator.second), 1));
    }

    return CollisionsPerTileIndex;
}
