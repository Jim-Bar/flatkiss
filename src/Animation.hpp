#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

#include <memory>

class Animation {
public:
    Animation(std::unique_ptr<uint16_t const[]> TileIndices, uint8_t Period, uint8_t Duration);
    uint8_t getDuration() const;
    uint8_t getPeriod() const;
    uint16_t tileIndexAtStep(uint16_t Step) const;

private:
    uint8_t const Duration;
    uint8_t const Period;
    std::unique_ptr<uint16_t const[]> TileIndices;
};

#endif
