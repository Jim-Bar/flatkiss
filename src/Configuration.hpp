#ifndef CONFIGURATION_HPP_INCLUDED
#define CONFIGURATION_HPP_INCLUDED

#include <string>

class Configuration {
public:
    Configuration(std::string const& configuration_file_path);
    std::string const& get_animations_path() const;
    size_t get_engine_tick_duration_ms() const;
    size_t get_level_height_in_tiles() const;
    std::string const& get_level_path() const;
    size_t get_level_width_in_tiles() const;
    size_t get_tileset_gap() const;
    size_t get_tileset_height_in_tiles() const;
    size_t get_tileset_left_offset() const;
    std::string const& get_tileset_path() const;
    size_t get_tileset_tiles_size() const;
    size_t get_tileset_top_offset() const;
    size_t get_tileset_width_in_tiles() const;

private:
    std::string animations_path;
    size_t engine_tick_duration_ms;
    size_t level_height_in_tiles;
    std::string level_path;
    size_t level_width_in_tiles;
    size_t tileset_gap;
    size_t tileset_height_in_tiles;
    size_t tileset_left_offset;
    std::string tileset_path;
    size_t tileset_tiles_size;
    size_t tileset_top_offset;
    size_t tileset_width_in_tiles;
};

#endif
