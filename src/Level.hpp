#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include <memory>
#include <string>
#include <vector>

/**
 * @brief Models a level.
 *
 * A level is a list of tiles indices, with dimensions. Once created, this class provides handy methods to access the
 * content of the level.
 */
class Level {
public:
    Level(std::vector<uint16_t> const& Tiles, size_t WidthInTiles, size_t HeightInTiles);
    size_t heightInTiles() const;
    uint16_t tileIndex(size_t i, size_t j) const;
    size_t widthInTiles() const;

private:
    size_t const HeightInTiles;
    std::vector<uint16_t> const Tiles;
    size_t const WidthInTiles;
};

class LevelLoader {
public:
    static std::unique_ptr<Level const> load(std::string const& Path, size_t WidthInTiles, size_t HeightInTiles);
};

#endif
