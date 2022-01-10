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
