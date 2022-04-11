#include "configuration.hpp"

#include <inipp.h>

#include <fstream>

using std::ifstream;
using std::string;

Configuration::Configuration(string const& file_path) {
  inipp::Ini<char> ini;
  ifstream configuration_input_stream(file_path);
  ini.parse(configuration_input_stream);

  /* The values are retrieved at construction time to fail early if something is
   * wrong. */
  inipp::get_value(ini.sections["Animations"], "path", animations_path_);
  inipp::get_value(ini.sections["Characters"], "characterset_files_directory",
                   characterset_files_directory_);
  inipp::get_value(ini.sections["Characters"], "characterset_files_prefix",
                   characterset_files_prefix_);
  inipp::get_value(ini.sections["Characters"], "characterset_files_suffix",
                   characterset_files_suffix_);
  inipp::get_value(ini.sections["Characters"], "charactersets_path",
                   charactersets_path_);
  inipp::get_value(ini.sections["Characters"], "path", characters_path_);
  inipp::get_value(ini.sections["Collisions"], "path", collisions_path_);
  inipp::get_value(ini.sections["Engine"], "tick_duration_ms",
                   engine_tick_duration_ms_);
  inipp::get_value(ini.sections["Level"], "height_in_tiles",
                   level_height_in_tiles_);
  inipp::get_value(ini.sections["Level"], "path", level_path_);
  inipp::get_value(ini.sections["Level"], "width_in_tiles",
                   level_width_in_tiles_);
  inipp::get_value(ini.sections["Tileset"], "gap", tileset_gap_);
  inipp::get_value(ini.sections["Tileset"], "height_in_tiles",
                   tileset_height_in_tiles_);
  inipp::get_value(ini.sections["Tileset"], "left_offset",
                   tileset_left_offset_);
  inipp::get_value(ini.sections["Tileset"], "path", tileset_path_);
  inipp::get_value(ini.sections["Tileset"], "tiles_size", tileset_tiles_size_);
  inipp::get_value(ini.sections["Tileset"], "top_offset", tileset_top_offset_);
  inipp::get_value(ini.sections["Tileset"], "width_in_tiles",
                   tileset_width_in_tiles_);
}

string Configuration::animationsPath() const {
  return animations_path_;
}

string Configuration::charactersetFilesDirectory() const {
  return characterset_files_directory_;
}

string Configuration::charactersetFilesPrefix() const {
  return characterset_files_prefix_;
}

string Configuration::charactersetFilesSuffix() const {
  return characterset_files_suffix_;
}

string const& Configuration::charactersetsPath() const {
  return charactersets_path_;
}

string const& Configuration::charactersPath() const { return characters_path_; }

string const& Configuration::collisionsPath() const { return collisions_path_; }

int64_t Configuration::engineTickDurationMs() const {
  return engine_tick_duration_ms_;
}

int64_t Configuration::levelHeightInTiles() const {
  return level_height_in_tiles_;
}

string const& Configuration::levelPath() const { return level_path_; }

int64_t Configuration::levelWidthInTiles() const {
  return level_width_in_tiles_;
}

int64_t Configuration::tilesetGap() const { return tileset_gap_; }

int64_t Configuration::tilesetHeightInTiles() const {
  return tileset_height_in_tiles_;
}

int64_t Configuration::tilesetLeftOffset() const {
  return tileset_left_offset_;
}

string const& Configuration::tilesetPath() const { return tileset_path_; }

int64_t Configuration::tilesetTilesSize() const { return tileset_tiles_size_; }

int64_t Configuration::tilesetTopOffset() const { return tileset_top_offset_; }

int64_t Configuration::tilesetWidthInTiles() const {
  return tileset_width_in_tiles_;
}
