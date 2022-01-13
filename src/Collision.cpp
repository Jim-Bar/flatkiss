#include <fstream>

#include "Collision.hpp"

Collision::Collision(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height) {

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
            CollisionsPerTileIndex.emplace(std::piecewise_construct, std::forward_as_tuple(TileIndex), std::forward_as_tuple(X, Y, Width, Height));
        }
        Stream.close();
    } // FIXME: Raise exception.

    return CollisionsPerTileIndex;
}
