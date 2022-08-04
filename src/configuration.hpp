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
  Configuration(std::string const& file_path);
  std::string const& actionSpriteMapsPath() const;
  std::string animationsPath() const;
  std::string const& charactersPath() const;
  int64_t engineTickDurationMs() const;
  int64_t levelHeightInTiles() const;
  std::string const& levelPath() const;
  int64_t levelWidthInTiles() const;
  std::string const& solidsPath() const;
  std::string spritesetFilesDirectory() const;
  std::string spritesetFilesPrefix() const;
  std::string spritesetFilesSuffix() const;
  std::string const& spritesetsPath() const;
  std::string const& tileSolidMapsPath() const;

 private:
  std::string action_sprite_maps_path_{};
  std::string animations_path_{};
  std::string characters_path_{};
  int64_t engine_tick_duration_ms_{0};
  int64_t level_height_in_tiles_{0};
  std::string level_path_{};
  int64_t level_width_in_tiles_{0};
  std::string solids_path_{};
  std::string spriteset_files_directory_{};
  std::string spriteset_files_prefix_{};
  std::string spriteset_files_suffix_{};
  std::string spritesets_path_{};
  std::string tile_solid_maps_path_{};
};

#endif
