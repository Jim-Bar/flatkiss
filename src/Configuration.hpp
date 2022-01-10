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
    std::string AnimationsPath;
    size_t EngineTickDurationMs;
    size_t LevelHeightInTiles;
    std::string LevelPath;
    size_t LevelWidthInTiles;
    size_t TilesetGap;
    size_t TilesetHeightInTiles;
    size_t TilesetLeftOffset;
    std::string TilesetPath;
    size_t TilesetTilesSize;
    size_t TilesetTopOffset;
    size_t TilesetWidthInTiles;
};

#endif
