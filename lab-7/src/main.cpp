#include "GetCoordinates.h"
#include "PrintWeather.h"
#include <string>
#include <cpr/cpr.h>
#include <vector>
#include <conio.h>
#include <map>
#include <chrono>

std::map<std::string, std::vector<GetWeather::ProcessingData>> CityLastWeather;
std::map<std::string, std::chrono::system_clock::time_point> lastUpdateTimes;


bool ParseConfigFile(std::ifstream& configFile, int& current_index_, int& current_amount_of_day_, int& fresh_frequency, std::vector<std::string>& cities) {
    try {
        if (!configFile.is_open()) {
            std::cerr << "Could not open config file." << std::endl;
            return false;
        }
        nlohmann::json j = nlohmann::json::parse(configFile);
        if (j.find("forecastDays") == j.end() || j.find("updateFrequency") == j.end() || j.find("cities") == j.end()) {
            std::cerr << "Required keys are not present in the config file." << std::endl;
            return false;
        }
        current_index_ =  0;
        current_amount_of_day_ = j["forecastDays"].get<int>();
        fresh_frequency = j["updateFrequency"].get<int>();
        cities = j["cities"].get<std::vector<std::string>>();

        return true;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    } catch (const nlohmann::json::type_error& e) {
        std::cerr << "JSON type error: " << e.what() << std::endl;
        return false;
    } catch (const nlohmann::json::out_of_range& e) {
        std::cerr << "JSON out of range error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}

int main() {
    int current_index_;
    int current_amount_of_day_;
    int fresh_frequency;
    std::vector<std::string> cities;

    std::ifstream configFile("C:\\weatherConfig.json");

    if (!ParseConfigFile(configFile, current_index_, current_amount_of_day_, fresh_frequency, cities)) {
        std::cerr << "Failed to parse config file." << std::endl;
        return 1;
    }
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == '+') {
                system("cls");
                if (current_amount_of_day_ < 16) current_amount_of_day_ += 1;
                std::string city_name_ = cities[current_index_];
                auto lastUpdateTime = lastUpdateTimes.find(city_name_);
                auto now = std::chrono::system_clock::now();

                if ((lastUpdateTime != lastUpdateTimes.end() &&
                    now - lastUpdateTime->second < std::chrono::minutes(fresh_frequency)) && (CityLastWeather[city_name_].size() >= current_amount_of_day_ * 4)) {
                    PrintWeather::ConsolePrinter(CityLastWeather[city_name_], city_name_, current_amount_of_day_);
                } else {
                    if (CityCoordinates.contains(city_name_)) {
                        CityLastWeather[city_name_].clear();
                        std::vector<GetWeather::ProcessingData> NowWeather = GetWeather::WeatherForecast(
                                CityCoordinates[city_name_].first, CityCoordinates[city_name_].second, current_amount_of_day_, city_name_);
                        PrintWeather::ConsolePrinter(NowWeather, city_name_, current_amount_of_day_);
                    } else {
                        std::pair<double, double> NowCoordinates = GeoPositional::coordinates(city_name_);
                        CityCoordinates[city_name_] = NowCoordinates;
                        std::vector<GetWeather::ProcessingData> NowWeather = GetWeather::WeatherForecast(
                                NowCoordinates.first, NowCoordinates.second, current_amount_of_day_, city_name_);
                        PrintWeather::ConsolePrinter(NowWeather, city_name_, current_amount_of_day_);
                    }
                    lastUpdateTimes[city_name_] = now;
                }
            } else if (key == '-') {
                system("cls");
                if (current_amount_of_day_ > 1)
                    current_amount_of_day_ -= 1;
                if (current_amount_of_day_ > 0) {
                    std::string city_name_ = cities[current_index_];
                    auto lastUpdateTime = lastUpdateTimes.find(city_name_);
                    auto now = std::chrono::system_clock::now();

                    if ((lastUpdateTime != lastUpdateTimes.end() &&
                         now - lastUpdateTime->second < std::chrono::minutes(fresh_frequency)) && (CityLastWeather[city_name_].size() >= current_amount_of_day_ * 4)) {
                        PrintWeather::ConsolePrinter(CityLastWeather[city_name_], city_name_, current_amount_of_day_);
                    } else {
                        if (CityCoordinates.contains(city_name_)) {
                            CityLastWeather[city_name_].clear();
                            std::vector<GetWeather::ProcessingData> NowWeather = GetWeather::WeatherForecast(
                                    CityCoordinates[city_name_].first, CityCoordinates[city_name_].second, current_amount_of_day_, city_name_);
                            PrintWeather::ConsolePrinter(NowWeather, city_name_, current_amount_of_day_);
                        } else {
                            std::pair<double, double> NowCoordinates = GeoPositional::coordinates(city_name_);
                            CityCoordinates[city_name_] = NowCoordinates;
                            std::vector<GetWeather::ProcessingData> NowWeather = GetWeather::WeatherForecast(
                                    NowCoordinates.first, NowCoordinates.second, current_amount_of_day_, city_name_);
                            PrintWeather::ConsolePrinter(NowWeather, city_name_, current_amount_of_day_);
                        }
                        lastUpdateTimes[city_name_] = now;

                    }
                }
            } else if (key == 'n') {
                system("cls");
                current_index_ = (current_index_ + 1) % cities.size();
                std::string city_name_ = cities[current_index_];
                auto lastUpdateTime = lastUpdateTimes.find(city_name_);
                auto now = std::chrono::system_clock::now();

                if ((lastUpdateTime != lastUpdateTimes.end() &&
                     now - lastUpdateTime->second < std::chrono::minutes(fresh_frequency)) && (CityLastWeather[city_name_].size() >= current_amount_of_day_ * 4)) {
                    PrintWeather::ConsolePrinter(CityLastWeather[city_name_], city_name_, current_amount_of_day_);
                } else {
                    if (CityCoordinates.contains(city_name_)) {
                        CityLastWeather[city_name_].clear();
                        std::vector<GetWeather::ProcessingData> NowWeather = GetWeather::WeatherForecast(
                                CityCoordinates[city_name_].first, CityCoordinates[city_name_].second, current_amount_of_day_, city_name_);
                        PrintWeather::ConsolePrinter(NowWeather, city_name_, current_amount_of_day_);
                    } else {
                        std::pair<double, double> NowCoordinates = GeoPositional::coordinates(city_name_);
                        CityCoordinates[city_name_] = NowCoordinates;
                        std::vector<GetWeather::ProcessingData> NowWeather = GetWeather::WeatherForecast(
                                NowCoordinates.first, NowCoordinates.second, current_amount_of_day_, city_name_);
                        PrintWeather::ConsolePrinter(NowWeather, city_name_, current_amount_of_day_);
                    }
                    lastUpdateTimes[city_name_] = now;
                }
            } else if (key == 'p') {
                system("cls");
                if (current_index_ > 0) {
                    --current_index_;
                } else {
                    current_index_ = cities.size() - 1;
                }
                std::string city_name_ = cities[current_index_];
                auto lastUpdateTime = lastUpdateTimes.find(city_name_);
                auto now = std::chrono::system_clock::now();

                if ((lastUpdateTime != lastUpdateTimes.end() &&
                     now - lastUpdateTime->second < std::chrono::minutes(fresh_frequency)) && (CityLastWeather[city_name_].size() >= current_amount_of_day_ * 4)) {
                    PrintWeather::ConsolePrinter(CityLastWeather[city_name_], city_name_, current_amount_of_day_);
                } else {
                    if (CityCoordinates.contains(city_name_)) {
                        CityLastWeather[city_name_].clear();
                        std::vector<GetWeather::ProcessingData> NowWeather = GetWeather::WeatherForecast(
                                CityCoordinates[city_name_].first, CityCoordinates[city_name_].second, current_amount_of_day_, city_name_);
                        PrintWeather::ConsolePrinter(NowWeather, city_name_, current_amount_of_day_);
                    } else {
                        std::pair<double, double> NowCoordinates = GeoPositional::coordinates(city_name_);
                        CityCoordinates[city_name_] = NowCoordinates;
                        std::vector<GetWeather::ProcessingData> NowWeather = GetWeather::WeatherForecast(
                                NowCoordinates.first, NowCoordinates.second, current_amount_of_day_, city_name_);
                        PrintWeather::ConsolePrinter(NowWeather, city_name_, current_amount_of_day_);
                    }
                    lastUpdateTimes[city_name_] = now;
                }
            } else if (key == 27) {
                break;
            }
        }
    }
    return 0;
}