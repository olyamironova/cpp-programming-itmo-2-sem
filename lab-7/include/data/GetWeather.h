#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstdint>
#include <cstring>
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include <stdexcept>

class GetWeather {
public:
    struct ProcessingData {
        std::string data;
        std::string part_of_day;
        std::pair<double, double> max_min_temperature;
        int relative_humidity_2m;
        double precipitation;
        int weather_code;
        double visibility;
        double wind_speed_10m;
        int wind_direction_10m;
        ProcessingData() : max_min_temperature(std::make_pair(-1000.0, 1000.0)), relative_humidity_2m(0),
                           precipitation(0.0), visibility(0.0), wind_speed_10m(0.0), wind_direction_10m(0) {}
    };
    static std::vector<ProcessingData> WeatherForecast(double latitude_, double longitude_, uint8_t amount_of_day, std::string& city_name);
};

extern std::map<std::string, std::vector<GetWeather::ProcessingData>> CityLastWeather;
