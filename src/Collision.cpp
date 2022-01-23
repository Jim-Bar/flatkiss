#include <fstream>

#include "Collision.hpp"

using std::make_unique;
using std::move;
using std::unique_ptr;

Collision::Collision(unique_ptr<PositionedRectangle const[]> PositionedRectangles, size_t RectanglesCount) : PositionedRectangles(move(PositionedRectangles)), RectanglesCount(RectanglesCount) {

}

bool Collision::collidesWith(PositionedRectangle const& PositionedRect, Position const WhenAtPosition) const {
    for (size_t I{0}; I < RectanglesCount; I++) {
        if (PositionedRect.intersectsWith(WhenAtPosition + PositionedRectangles[I])) {
            return true;
        }
    }

    return false;
}

std::unordered_map<uint16_t, Collision const> CollisionLoader::load(std::string const& FilePath) {
    std::ifstream Stream;
    std::unordered_map<uint16_t, Collision const> CollisionsPerTileIndex;
    Stream.open(FilePath, std::ios::in | std::ios::binary);
    if (Stream.is_open()) {
        // TODO: Use peek() for Animation and Level too.
        while (Stream.peek() != std::istream::traits_type::eof()) {
            uint16_t TileIndex{0};
            Stream.read(reinterpret_cast<char*>(&TileIndex), 2); // Two bytes per tile index.
            Position Position{static_cast<uint8_t>(Stream.get()), static_cast<uint8_t>(Stream.get())};
            Rectangle Rectangle{static_cast<uint8_t>(Stream.get()), static_cast<uint8_t>(Stream.get())};
            // FIXME: Use C++20: https://stackoverflow.com/a/55144743
            // FIXME: Several rectangles by collision.
            auto PositionedRectangles = unique_ptr<PositionedRectangle const[]>(new PositionedRectangle const[1] {PositionedRectangle{Position, Rectangle}});
            CollisionsPerTileIndex.emplace(std::piecewise_construct, std::forward_as_tuple(TileIndex), std::forward_as_tuple(move(PositionedRectangles), 1)); // FIXME: fix.
        }
        Stream.close();
    } // FIXME: Raise exception.

    return CollisionsPerTileIndex;
}
