#include "GetCoordinates.h"
std::map<std::string, std::pair<double, double>> CityCoordinates;

std::pair<double, double> GeoPositional::coordinates(const std::string& city_name) {
cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city?name=" + city_name },
                            cpr::Header{ {"X-Api-Key", "paste-your-personal-secret-key-here)))"} });
nlohmann::json j = nlohmann::json::parse(r.text);
double latitude = j[0]["latitude"];
double longitude = j[0]["longitude"];
CityCoordinates[city_name] = std::make_pair(latitude, longitude);
return std::make_pair(latitude, longitude);
}