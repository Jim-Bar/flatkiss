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
  std::string animations_path_{};
  std::string collisions_path_{};
  size_t engine_tick_duration_ms_{0};
  size_t level_height_in_tiles_{0};
  std::string level_path_{};
  size_t level_width_in_tiles_{0};
  size_t tileset_gap_{0};
  size_t tileset_height_in_tiles_{0};
  size_t tileset_left_offset_{0};
  std::string tileset_path_{};
  size_t tileset_tiles_size_{0};
  size_t tileset_top_offset_{0};
  size_t tileset_width_in_tiles_{0};
};

#endif
