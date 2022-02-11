#include <fstream>

#include "Animation.hpp"

Animation::Animation(std::vector<uint16_t> const& TileIndices, uint8_t Period, uint8_t Duration) :
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
        while (Stream.peek() != std::istream::traits_type::eof()) {
            uint8_t AnimationPeriod{static_cast<uint8_t>(Stream.get())};
            uint8_t AnimationDuration{static_cast<uint8_t>(Stream.get())};
            /* The vector containing animations is created and space is reserved for
             * containing all of them at the same time. Then the stream is read
             * directly into the vector. */
            auto Animation{std::vector<uint16_t>(AnimationPeriod, 0)};
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            Stream.read(reinterpret_cast<char*>(Animation.data()),
                        AnimationPeriod * 2);  // Two bytes per tile.
            AnimationsPerTileIndex.emplace(
                std::piecewise_construct, std::forward_as_tuple(Animation[0]),
                std::forward_as_tuple(std::move(Animation), AnimationPeriod,
                                      AnimationDuration));
        }
        Stream.close();
    } // FIXME: Raise exception.

    return AnimationsPerTileIndex;
}
