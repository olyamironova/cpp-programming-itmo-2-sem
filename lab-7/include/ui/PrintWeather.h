#pragma once
#include "GetWeather.h"
#include "GetCoordinates.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdint>
#include <ctime>
#include <unordered_map>

class PrintWeather {
public:
    static void ConsolePrinter(const std::vector<GetWeather::ProcessingData>& WeatherTable, const std::string& CityName, uint8_t amount_of_day);
};