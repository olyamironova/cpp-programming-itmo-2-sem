#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

class ConfigParser {
public:
    struct Config {
        static uint8_t updateFrequency;
        static uint8_t forecastDays;
        static std::vector<std::string> cities;
    };

    static void loadConfigFromFile(const std::string& filename, Config& config);
};
