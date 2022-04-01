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
  std::string const& animationsPath() const;
  std::string const& charactersetsPath() const;
  std::string const& charactersPath() const;
  std::string charactersetPath(int64_t characterset) const;
  std::string const& collisionsPath() const;
  int64_t engineTickDurationMs() const;
  int64_t levelHeightInTiles() const;
  std::string const& levelPath() const;
  int64_t levelWidthInTiles() const;
  int64_t tilesetGap() const;
  int64_t tilesetHeightInTiles() const;
  int64_t tilesetLeftOffset() const;
  std::string const& tilesetPath() const;
  int64_t tilesetTilesSize() const;
  int64_t tilesetTopOffset() const;
  int64_t tilesetWidthInTiles() const;

 private:
  std::string animations_path_{};
  std::string charactersets_path_{};
  std::string characters_path_{};
  std::string characterset_files_directory_{};
  std::string characterset_files_prefix_{};
  std::string characterset_files_suffix_{};
  std::string collisions_path_{};
  int64_t engine_tick_duration_ms_{0};
  int64_t level_height_in_tiles_{0};
  std::string level_path_{};
  int64_t level_width_in_tiles_{0};
  int64_t tileset_gap_{0};
  int64_t tileset_height_in_tiles_{0};
  int64_t tileset_left_offset_{0};
  std::string tileset_path_{};
  int64_t tileset_tiles_size_{0};
  int64_t tileset_top_offset_{0};
  int64_t tileset_width_in_tiles_{0};
};

#endif
