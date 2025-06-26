#include "PrintWeather.h"

void PrintWeather::ConsolePrinter(const std::vector<GetWeather::ProcessingData>& WeatherTable, const std::string& CityName, uint8_t amount_of_day) {
    std::cout << "------------------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(((132 - CityName.size()) / 2) + 12) << CityName << "\n";
    for (size_t i = 0; i < amount_of_day; ++i) { // 66
            std::cout << "------------------------------------------------------------------------------------------------------------------------------------\n";
            std::cout << std::setw(71) << WeatherTable[4 * i].data << "\n";
            std::cout << "------------------------------------------------------------------------------------------------------------------------------------\n";
            std::cout << "|" << std::setw(18) << "Night" << std::setw(15) << "|" << std::setw(18) << "Morning" << std::setw(15) << "|" << std::setw(18) << "Afternoon" << std::setw(15) << "|" << std::setw(18) << "Evening" << std::setw(15) << "|" << "\n";
            std::cout << "------------------------------------------------------------------------------------------------------------------------------------\n";
            // Temperature: -26.8°C(-29.7°C) 20
            for (size_t j = 0; j < 4; ++j) {
                std::cout << "|" << "Temperature: ";
                int length_ = 0;
                if (static_cast<int>(WeatherTable[4 * i + j].max_min_temperature.first) > 0) { std::cout << "+"; length_ += 1; }
                std::cout << WeatherTable[4 * i + j].max_min_temperature.first << '\370' << "C" << "(";
                length_ += std::to_string(static_cast<int>(WeatherTable[4 * i + j].max_min_temperature.first)).size();
                if (static_cast<int>(WeatherTable[4 * i + j].max_min_temperature.first) > 0) { std::cout << "+"; length_ += 1; }
                length_ += std::to_string(static_cast<int>(WeatherTable[4 * i + j].max_min_temperature.second)).size();
                length_ += 10;
                std::cout << WeatherTable[4 * i + j].max_min_temperature.second << '\370' << "C" << ")";
                std::cout << std::setw(20 - length_);
            }
            std::cout << "|" << "\n"; // 17
            for (size_t j = 0; j < 4; ++j) {
                int length_ = 0;
                std::cout << "|" << "Wind Speed: ";
                printf("%2.1f", WeatherTable[4 * i + j].wind_speed_10m);
                length_ += 2;
                length_ += std::to_string(static_cast<int>(WeatherTable[4 * i + j].wind_speed_10m)).size();
                std::cout << " km/h" << std::setw(16 - length_);
            }
            std::cout << "|" << "\n";
            for (size_t j = 0; j < 4; ++j) {
                std::cout << "|" << "Humidity: ";
                if (WeatherTable[4 * i + j].relative_humidity_2m > 100) {
                    std::cout << 100 << " %" << std::setw(18);
                } else if (WeatherTable[4 * i + j].relative_humidity_2m < 10) {
                    std::cout << WeatherTable[4 * i + j].relative_humidity_2m << " %" << std::setw(20);
                } else {
                    std::cout << WeatherTable[4 * i + j].relative_humidity_2m << " %" << std::setw(19);
                }
            }
            std::cout << "|" << "\n";
            for (size_t j = 0; j < 4; ++j) {
                std::cout << "|" << "Precipitation: ";
                std::cout << WeatherTable[4 * i + j].precipitation << " mm" << std::setw(22 - ((std::to_string(WeatherTable[4 * i + j].precipitation)).size()));
            }
            std::cout << "|" << "\n";
            /*std::cout << "Wind speed: " << WeatherTable[i].wind_speed_10m << " km/h" << "\n";
            std::cout << "Precipitation: " << WeatherTable[i].precipitation << " mm" << "\n";
            std::cout << "Visibility: " << WeatherTable[i].visibility << " m" << "\n";
            std::cout << "Humidity: " << WeatherTable[i].relative_humidity_2m << " %" << "\n";
            std::string forecast = code_to_forecast.at(static_cast<uint64_t>(jsonData["hourly"]["weather_code"][i]));
            std::array<std::string, 5> myArray = forecast_to_image.at(forecast);
            for (const auto& element : myArray) {
                std::cout << element << "\n";
            }
            std::cout << code_to_forecast.at(static_cast<uint64_t>(jsonData["hourly"]["weather_code"][i])) << "\n";*/
        }
    std::cout << "------------------------------------------------------------------------------------------------------------------------------------\n";
        system("pause");
}