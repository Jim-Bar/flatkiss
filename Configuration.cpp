#include <inipp.h>
#include <fstream>

#include "Configuration.hpp"

Configuration::Configuration(std::string const& configuration_file_path) {
    inipp::Ini<char> ini;
    std::ifstream configuration_input_stream(configuration_file_path);
    ini.parse(configuration_input_stream);
    inipp::get_value(ini.sections["Engine"], "tick_duration_ms", _engine_tick_duration_ms);
}

size_t Configuration::engine_tick_duration_ms() const {
    return _engine_tick_duration_ms;
}
