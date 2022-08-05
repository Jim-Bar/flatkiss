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
  inipp::get_value(ini.sections["ActionSpriteMaps"], "path",
                   action_sprite_maps_path_);
  inipp::get_value(ini.sections["Animations"], "path", animations_path_);
  inipp::get_value(ini.sections["Characters"], "path", characters_path_);
  inipp::get_value(ini.sections["Engine"], "tick_duration_ms",
                   engine_tick_duration_ms_);
  inipp::get_value(ini.sections["Levels"], "path", levels_path_);
  inipp::get_value(ini.sections["Solids"], "path", solids_path_);
  inipp::get_value(ini.sections["Sprites"], "files_directory",
                   spriteset_files_directory_);
  inipp::get_value(ini.sections["Sprites"], "files_prefix",
                   spriteset_files_prefix_);
  inipp::get_value(ini.sections["Sprites"], "files_suffix",
                   spriteset_files_suffix_);
  inipp::get_value(ini.sections["Sprites"], "path", spritesets_path_);
  inipp::get_value(ini.sections["TileSolidMaps"], "path",
                   tile_solid_maps_path_);
}

string const& Configuration::actionSpriteMapsPath() const {
  return action_sprite_maps_path_;
}

string Configuration::animationsPath() const { return animations_path_; }

string const& Configuration::charactersPath() const { return characters_path_; }

int64_t Configuration::engineTickDurationMs() const {
  return engine_tick_duration_ms_;
}

string const& Configuration::levelsPath() const { return levels_path_; }

string Configuration::spritesetFilesDirectory() const {
  return spriteset_files_directory_;
}

string Configuration::spritesetFilesPrefix() const {
  return spriteset_files_prefix_;
}

string Configuration::spritesetFilesSuffix() const {
  return spriteset_files_suffix_;
}

string const& Configuration::spritesetsPath() const { return spritesets_path_; }

string const& Configuration::solidsPath() const { return solids_path_; }

string const& Configuration::tileSolidMapsPath() const {
  return tile_solid_maps_path_;
}
