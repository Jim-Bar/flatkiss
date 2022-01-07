#include <inipp.h>
#include <fstream>

#include "Configuration.hpp"

Configuration::Configuration(std::string const& file_path) {
    inipp::Ini<char> ini;
    std::ifstream configuration_input_stream(file_path);
    ini.parse(configuration_input_stream);

    // The values are retrieved at construction time to fail early if something is wrong.
    inipp::get_value(ini.sections["Animations"], "path", AnimationsPath);
    inipp::get_value(ini.sections["Engine"], "tick_duration_ms", EngineTickDurationMs);
    inipp::get_value(ini.sections["Level"], "height_in_tiles", LevelHeightInTiles);
    inipp::get_value(ini.sections["Level"], "path", LevelPath);
    inipp::get_value(ini.sections["Level"], "width_in_tiles", LevelWidthInTiles);
    inipp::get_value(ini.sections["Tileset"], "gap", TilesetGap);
    inipp::get_value(ini.sections["Tileset"], "height_in_tiles", TilesetHeightInTiles);
    inipp::get_value(ini.sections["Tileset"], "left_offset", TilesetLeftOffset);
    inipp::get_value(ini.sections["Tileset"], "path", TilesetPath);
    inipp::get_value(ini.sections["Tileset"], "tiles_size", TilesetTilesSize);
    inipp::get_value(ini.sections["Tileset"], "top_offset", TilesetTopOffset);
    inipp::get_value(ini.sections["Tileset"], "width_in_tiles", TilesetWidthInTiles);
}

std::string const& Configuration::animationsPath() const {
    return AnimationsPath;
}

size_t Configuration::engineTickDurationMs() const {
    return EngineTickDurationMs;
}

size_t Configuration::levelHeightInTiles() const {
    return LevelHeightInTiles;
}

std::string const& Configuration::levelPath() const {
    return LevelPath;
}

size_t Configuration::levelWidthInTiles() const {
    return LevelWidthInTiles;
}

size_t Configuration::tilesetGap() const {
    return TilesetGap;
}

size_t Configuration::tilesetHeightInTiles() const {
    return TilesetHeightInTiles;
}

size_t Configuration::tilesetLeftOffset() const {
    return TilesetLeftOffset;
}
std::string const& Configuration::tilesetPath() const {
    return TilesetPath;
}

size_t Configuration::tilesetTilesSize() const {
    return TilesetTilesSize;
}

size_t Configuration::tilesetTopOffset() const {
    return TilesetTopOffset;
}

size_t Configuration::tilesetWidthInTiles() const {
    return TilesetWidthInTiles;
}
