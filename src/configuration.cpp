#include "configuration.hpp"

#include <inipp.h>

#include <fstream>

Configuration::Configuration(std::string const& FilePath) {
  inipp::Ini<char> Ini;
  std::ifstream ConfigurationInputStream(FilePath);
  Ini.parse(ConfigurationInputStream);

  /* The values are retrieved at construction time to fail early if something is
   * wrong. */
  inipp::get_value(Ini.sections["Animations"], "path", AnimationsPath);
  inipp::get_value(Ini.sections["Collisions"], "path", CollisionsPath);
  inipp::get_value(Ini.sections["Engine"], "tick_duration_ms",
                   EngineTickDurationMs);
  inipp::get_value(Ini.sections["Level"], "height_in_tiles",
                   LevelHeightInTiles);
  inipp::get_value(Ini.sections["Level"], "path", LevelPath);
  inipp::get_value(Ini.sections["Level"], "width_in_tiles", LevelWidthInTiles);
  inipp::get_value(Ini.sections["Tileset"], "gap", TilesetGap);
  inipp::get_value(Ini.sections["Tileset"], "height_in_tiles",
                   TilesetHeightInTiles);
  inipp::get_value(Ini.sections["Tileset"], "left_offset", TilesetLeftOffset);
  inipp::get_value(Ini.sections["Tileset"], "path", TilesetPath);
  inipp::get_value(Ini.sections["Tileset"], "tiles_size", TilesetTilesSize);
  inipp::get_value(Ini.sections["Tileset"], "top_offset", TilesetTopOffset);
  inipp::get_value(Ini.sections["Tileset"], "width_in_tiles",
                   TilesetWidthInTiles);
}

std::string const& Configuration::animationsPath() const {
  return AnimationsPath;
}

std::string const& Configuration::collisionsPath() const {
  return CollisionsPath;
}

size_t Configuration::engineTickDurationMs() const {
  return EngineTickDurationMs;
}

size_t Configuration::levelHeightInTiles() const { return LevelHeightInTiles; }

std::string const& Configuration::levelPath() const { return LevelPath; }

size_t Configuration::levelWidthInTiles() const { return LevelWidthInTiles; }

size_t Configuration::tilesetGap() const { return TilesetGap; }

size_t Configuration::tilesetHeightInTiles() const {
  return TilesetHeightInTiles;
}

size_t Configuration::tilesetLeftOffset() const { return TilesetLeftOffset; }
std::string const& Configuration::tilesetPath() const { return TilesetPath; }

size_t Configuration::tilesetTilesSize() const { return TilesetTilesSize; }

size_t Configuration::tilesetTopOffset() const { return TilesetTopOffset; }

size_t Configuration::tilesetWidthInTiles() const {
  return TilesetWidthInTiles;
}
