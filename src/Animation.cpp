#include <fstream>

#include "Animation.hpp"

Animation::Animation(std::unique_ptr<uint16_t const[]> TileIndices, uint8_t Period, uint8_t Duration) :
    TileIndices(std::move(TileIndices)), Period(Period), Duration(Duration) {

}

uint8_t Animation::getDuration() const {
    return Duration;
}

uint8_t Animation::getPeriod() const {
    return Period;
}

uint16_t Animation::tileIndexAtStep(uint16_t Step) const {
    return TileIndices[Step];
}

std::unordered_map<uint16_t, Animation const> AnimationLoader::load(std::string const& FilePath) {
    std::ifstream Stream;
    std::unordered_map<uint16_t, Animation const> AnimationsPerTileIndex;
    Stream.open(FilePath, std::ios::in | std::ios::binary);
    if (Stream.is_open()) {
        char Byte{0};
        while ((Byte = Stream.get()) != std::istream::traits_type::eof()) {
            uint8_t AnimationPeriod{static_cast<uint8_t>(Byte)};
            uint8_t AnimationDuration{static_cast<uint8_t>(Stream.get())};
            auto Animation = std::make_unique<uint16_t[]>(AnimationPeriod);
            Stream.read(reinterpret_cast<char*>(Animation.get()), AnimationPeriod * 2); // Two byte per tile.
            AnimationsPerTileIndex.emplace(std::piecewise_construct, std::forward_as_tuple(Animation[0]), std::forward_as_tuple(std::move(Animation), AnimationPeriod, AnimationDuration));
        }
        Stream.close();
    } // FIXME: Raise exception.

    return AnimationsPerTileIndex;
}
