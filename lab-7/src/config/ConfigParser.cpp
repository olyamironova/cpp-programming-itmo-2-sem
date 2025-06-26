#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

class ConfigParser {
public:
    struct Config {
        static uint8_t updateFrequency;
        static uint8_t forecastDays;
        static std::vector<std::string> cities;
    };

    void loadConfigFromFile(const std::string& filename, Config& config) {
        std::ifstream configFile(filename);
        if (!configFile.is_open()) {
            throw std::runtime_error("Could not open config file.");
        }
        nlohmann::json j = nlohmann::json::parse(configFile);
        config.updateFrequency = j["updateFrequency"].get<int>();
        config.forecastDays = j["forecastDays"].get<int>();
        config.cities = j["cities"].get<std::vector<std::string>>();
        configFile.close();
    }
};



