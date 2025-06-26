#pragma once
#include <string>
#include <utility> // Для std::pair
#include <nlohmann/json.hpp> // Для nlohmann::json
#include <cpr/cpr.h> // Для cpr::Response и cpr::Get
#include <iostream>

class GeoPositional {
private:
    std::string city;
    const std::string api_key = "paste-your-personal-secret-key-here)))";
public:
    GeoPositional(const std::string& city_name) : city(city_name) {};
    static std::pair<double, double> coordinates(const std::string& city_name);
};

extern std::map<std::string, std::pair<double, double>> CityCoordinates;