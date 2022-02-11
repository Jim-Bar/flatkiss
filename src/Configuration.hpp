#ifndef CONFIGURATION_HPP_INCLUDED
#define CONFIGURATION_HPP_INCLUDED

#include <string>

/**
 * @brief Provider of configurable constants.
 *
 * Loads a configuration file in the INI style, and provides getters on the
 * constants from that file.
 */
class Configuration {
public:
    Configuration(std::string const& FilePath);
    std::string const& animationsPath() const;
    std::string const& collisionsPath() const;
    size_t engineTickDurationMs() const;
    size_t levelHeightInTiles() const;
    std::string const& levelPath() const;
    size_t levelWidthInTiles() const;
    size_t tilesetGap() const;
    size_t tilesetHeightInTiles() const;
    size_t tilesetLeftOffset() const;
    std::string const& tilesetPath() const;
    size_t tilesetTilesSize() const;
    size_t tilesetTopOffset() const;
    size_t tilesetWidthInTiles() const;

private:
    std::string AnimationsPath{};
    std::string CollisionsPath{};
    size_t EngineTickDurationMs{0};
    size_t LevelHeightInTiles{0};
    std::string LevelPath{};
    size_t LevelWidthInTiles{0};
    size_t TilesetGap{0};
    size_t TilesetHeightInTiles{0};
    size_t TilesetLeftOffset{0};
    std::string TilesetPath{};
    size_t TilesetTilesSize{0};
    size_t TilesetTopOffset{0};
    size_t TilesetWidthInTiles{0};
};

#endif
