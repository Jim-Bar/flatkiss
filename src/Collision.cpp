#include <fstream>

#include "Collision.hpp"

using std::move;
using std::unique_ptr;

Collision::Collision(unique_ptr<Rectangle const[]> Rectangles, size_t RectanglesCount) : Rectangles(move(Rectangles)), RectanglesCount(RectanglesCount) {

}

bool Collision::collidesWith(PositionedRectangle const& PositionedRect, Position const WhenAtPosition) const {
    for (size_t I{0}; I < RectanglesCount; I++) {
        if (PositionedRect.intersectsWith(PositionedRectangle(WhenAtPosition, Rectangles[I]))) {
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
            uint8_t X{static_cast<uint8_t>(Stream.get())};
            uint8_t Y{static_cast<uint8_t>(Stream.get())};
            uint8_t Width{static_cast<uint8_t>(Stream.get())};
            uint8_t Height{static_cast<uint8_t>(Stream.get())};
            CollisionsPerTileIndex.emplace(std::piecewise_construct, std::forward_as_tuple(TileIndex), std::forward_as_tuple(X, Y, Width, Height)); // FIXME: fix.
        }
        Stream.close();
    } // FIXME: Raise exception.

    return CollisionsPerTileIndex;
}
