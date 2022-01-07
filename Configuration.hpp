#ifndef CONFIGURATION_HPP_INCLUDED
#define CONFIGURATION_HPP_INCLUDED

#include <string>

class Configuration {
public:
    Configuration(std::string const& configuration_file_path);
    size_t engine_tick_duration_ms() const;

private:
    size_t _engine_tick_duration_ms;
};

#endif
