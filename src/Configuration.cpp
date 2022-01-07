#include <inipp.h>
#include <fstream>

#include "Configuration.hpp"

Configuration::Configuration(std::string const& configuration_file_path) {
    inipp::Ini<char> ini;
    std::ifstream configuration_input_stream(configuration_file_path);
    ini.parse(configuration_input_stream);

    // The values are retrieved at construction time to fail early if something is wrong.
    inipp::get_value(ini.sections["Animations"], "path", animations_path);
    inipp::get_value(ini.sections["Engine"], "tick_duration_ms", engine_tick_duration_ms);
    inipp::get_value(ini.sections["Level"], "height_in_tiles", level_height_in_tiles);
    inipp::get_value(ini.sections["Level"], "path", level_path);
    inipp::get_value(ini.sections["Level"], "width_in_tiles", level_width_in_tiles);
    inipp::get_value(ini.sections["Tileset"], "gap", tileset_gap);
    inipp::get_value(ini.sections["Tileset"], "height_in_tiles", tileset_height_in_tiles);
    inipp::get_value(ini.sections["Tileset"], "left_offset", tileset_left_offset);
    inipp::get_value(ini.sections["Tileset"], "path", tileset_path);
    inipp::get_value(ini.sections["Tileset"], "tiles_size", tileset_tiles_size);
    inipp::get_value(ini.sections["Tileset"], "top_offset", tileset_top_offset);
    inipp::get_value(ini.sections["Tileset"], "width_in_tiles", tileset_width_in_tiles);
}

std::string const& Configuration::get_animations_path() const {
    return animations_path;
}

size_t Configuration::get_engine_tick_duration_ms() const {
    return engine_tick_duration_ms;
}

size_t Configuration::get_level_height_in_tiles() const {
    return level_height_in_tiles;
}

std::string const& Configuration::get_level_path() const {
    return level_path;
}

size_t Configuration::get_level_width_in_tiles() const {
    return level_width_in_tiles;
}

size_t Configuration::get_tileset_gap() const {
    return tileset_gap;
}

size_t Configuration::get_tileset_height_in_tiles() const {
    return tileset_height_in_tiles;
}

size_t Configuration::get_tileset_left_offset() const {
    return tileset_left_offset;
}
std::string const& Configuration::get_tileset_path() const {
    return tileset_path;
}

size_t Configuration::get_tileset_tiles_size() const {
    return tileset_tiles_size;
}

size_t Configuration::get_tileset_top_offset() const {
    return tileset_top_offset;
}

size_t Configuration::get_tileset_width_in_tiles() const {
    return tileset_width_in_tiles;
}
