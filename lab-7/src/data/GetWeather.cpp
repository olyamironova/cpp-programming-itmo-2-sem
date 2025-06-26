#include "GetWeather.h"

std::vector<GetWeather::ProcessingData> GetWeather::WeatherForecast(double latitude_, double longitude_, uint8_t amount_of_day, std::string& city_name) {
    nlohmann::json jsonData;
    try {
        const std::string full_city_api_address =
                "https://api.open-meteo.com/v1/forecast?latitude=" + std::to_string(latitude_) + "&longitude=" +
                std::to_string(longitude_) +
                "&hourly=temperature_2m,relative_humidity_2m,precipitation,weather_code,visibility,wind_speed_10m,wind_direction_10m&forecast_days=" +
                std::to_string(amount_of_day);
        cpr::Response response = cpr::Get(cpr::Url{full_city_api_address},
                                          cpr::Header{{"X-Api-Key", "paste-your-personal-secret-key-here)))"}});

        if (response.status_code ==  200) {
            nlohmann::json jsonData = nlohmann::json::parse(response.text);
            std::vector<GetWeather::ProcessingData> weatherTable;
            GetWeather::ProcessingData processingData;
            for (size_t i =  0; i < jsonData["hourly"]["time"].size(); ++i) {
                switch (std::stoi(static_cast<std::string>(jsonData["hourly"]["time"][i]).substr(11, 13))) {
                    case 6:
                        processingData.data = static_cast<std::string>(jsonData["hourly"]["time"][i]).substr(0, 10);
                        processingData.part_of_day = "Night";
                        processingData.max_min_temperature.first = std::max(static_cast<double>(processingData.max_min_temperature.first), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.max_min_temperature.second = std::min(static_cast<double>(processingData.max_min_temperature.second), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.relative_humidity_2m = (processingData.relative_humidity_2m + static_cast<int>(jsonData["hourly"]["relative_humidity_2m"][i])) / 6;
                        processingData.precipitation = (processingData.precipitation + static_cast<double>(jsonData["hourly"]["precipitation"][i])) / 6.0;
                        processingData.weather_code = jsonData["hourly"]["weather_code"][i];
                        processingData.visibility = (processingData.visibility + static_cast<double>(jsonData["hourly"]["visibility"][i])) / 6;
                        processingData.wind_speed_10m = (processingData.wind_speed_10m + static_cast<double>(jsonData["hourly"]["wind_speed_10m"][i])) / 6.0;
                        processingData.wind_direction_10m = (processingData.wind_direction_10m + static_cast<int>(jsonData["hourly"]["wind_direction_10m"][i])) / 6;
                        weatherTable.push_back(processingData);
                        CityLastWeather[city_name].push_back(processingData);

                        processingData.max_min_temperature.first = static_cast<double>(jsonData["hourly"]["temperature_2m"][i]);
                        processingData.max_min_temperature.second = static_cast<double>(jsonData["hourly"]["temperature_2m"][i]);
                        processingData.relative_humidity_2m = static_cast<int>(jsonData["hourly"]["relative_humidity_2m"][i]);
                        processingData.precipitation = static_cast<double>(jsonData["hourly"]["precipitation"][i]);
                        processingData.weather_code = 0;
                        processingData.visibility = static_cast<double>(jsonData["hourly"]["visibility"][i]);
                        processingData.wind_speed_10m = static_cast<double>(jsonData["hourly"]["wind_speed_10m"][i]);
                        processingData.wind_direction_10m = static_cast<int>(jsonData["hourly"]["wind_direction_10m"][i]);


                        break;

                    case 12:
                        processingData.data = static_cast<std::string>(jsonData["hourly"]["time"][i]).substr(0, 10);
                        processingData.part_of_day = "Morning";
                        processingData.max_min_temperature.first = std::max(static_cast<double>(processingData.max_min_temperature.first), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.max_min_temperature.second = std::min(static_cast<double>(processingData.max_min_temperature.second), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.relative_humidity_2m = (processingData.relative_humidity_2m + static_cast<int>(jsonData["hourly"]["relative_humidity_2m"][i])) / 6;
                        processingData.precipitation = (processingData.precipitation + static_cast<double>(jsonData["hourly"]["precipitation"][i])) / 6.0;
                        processingData.weather_code = jsonData["hourly"]["weather_code"][i];
                        processingData.visibility = (processingData.visibility + static_cast<double>(jsonData["hourly"]["visibility"][i])) / 6;
                        processingData.wind_speed_10m = (processingData.wind_speed_10m + static_cast<double>(jsonData["hourly"]["wind_speed_10m"][i])) / 6.0;
                        processingData.wind_direction_10m = (processingData.wind_direction_10m + static_cast<int>(jsonData["hourly"]["wind_direction_10m"][i])) / 6;
                        weatherTable.push_back(processingData);
                        CityLastWeather[city_name].push_back(processingData);

                        processingData.max_min_temperature.first = static_cast<double>(jsonData["hourly"]["temperature_2m"][i]);
                        processingData.max_min_temperature.second = static_cast<double>(jsonData["hourly"]["temperature_2m"][i]);
                        processingData.relative_humidity_2m = static_cast<int>(jsonData["hourly"]["relative_humidity_2m"][i]);
                        processingData.precipitation = static_cast<double>(jsonData["hourly"]["precipitation"][i]);
                        processingData.weather_code = 0;
                        processingData.visibility = static_cast<double>(jsonData["hourly"]["visibility"][i]);
                        processingData.wind_speed_10m = static_cast<double>(jsonData["hourly"]["wind_speed_10m"][i]);
                        processingData.wind_direction_10m = static_cast<int>(jsonData["hourly"]["wind_direction_10m"][i]);

                        break;

                    case 18:
                        processingData.data = static_cast<std::string>(jsonData["hourly"]["time"][i]).substr(0, 10);
                        processingData.part_of_day = "Afternoon";
                        processingData.max_min_temperature.first = std::max(static_cast<double>(processingData.max_min_temperature.first), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.max_min_temperature.second = std::min(static_cast<double>(processingData.max_min_temperature.second), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.relative_humidity_2m = (processingData.relative_humidity_2m + static_cast<int>(jsonData["hourly"]["relative_humidity_2m"][i])) / 6;
                        processingData.precipitation = (processingData.precipitation + static_cast<double>(jsonData["hourly"]["precipitation"][i])) / 6.0;
                        processingData.weather_code = jsonData["hourly"]["weather_code"][i];
                        processingData.visibility = (processingData.visibility + static_cast<double>(jsonData["hourly"]["visibility"][i])) / 6;
                        processingData.wind_speed_10m = (processingData.wind_speed_10m + static_cast<double>(jsonData["hourly"]["wind_speed_10m"][i])) / 6.0;
                        processingData.wind_direction_10m = (processingData.wind_direction_10m + static_cast<int>(jsonData["hourly"]["wind_direction_10m"][i])) / 6;
                        weatherTable.push_back(processingData);
                        CityLastWeather[city_name].push_back(processingData);


                        processingData.max_min_temperature.first = static_cast<double>(jsonData["hourly"]["temperature_2m"][i]);
                        processingData.max_min_temperature.second = static_cast<double>(jsonData["hourly"]["temperature_2m"][i]);
                        processingData.relative_humidity_2m = static_cast<int>(jsonData["hourly"]["relative_humidity_2m"][i]);
                        processingData.precipitation = static_cast<double>(jsonData["hourly"]["precipitation"][i]);
                        processingData.weather_code = 0;
                        processingData.visibility = static_cast<double>(jsonData["hourly"]["visibility"][i]);
                        processingData.wind_speed_10m = static_cast<double>(jsonData["hourly"]["wind_speed_10m"][i]);
                        processingData.wind_direction_10m = static_cast<int>(jsonData["hourly"]["wind_direction_10m"][i]);

                        break;

                    case 23:
                        processingData.data = static_cast<std::string>(jsonData["hourly"]["time"][i]).substr(0, 10);
                        processingData.part_of_day = "Evening";
                        processingData.max_min_temperature.first = std::max(static_cast<double>(processingData.max_min_temperature.first), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.max_min_temperature.second = std::min(static_cast<double>(processingData.max_min_temperature.second), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.relative_humidity_2m = (processingData.relative_humidity_2m + static_cast<int>(jsonData["hourly"]["relative_humidity_2m"][i])) / 6;
                        processingData.precipitation = (processingData.precipitation + static_cast<double>(jsonData["hourly"]["precipitation"][i])) / 6.0;
                        processingData.weather_code = jsonData["hourly"]["weather_code"][i];
                        processingData.visibility = (processingData.visibility + static_cast<double>(jsonData["hourly"]["visibility"][i])) / 6;
                        processingData.wind_speed_10m = (processingData.wind_speed_10m + static_cast<double>(jsonData["hourly"]["wind_speed_10m"][i])) / 6.0;
                        processingData.wind_direction_10m = (processingData.wind_direction_10m + static_cast<int>(jsonData["hourly"]["wind_direction_10m"][i])) / 6;
                        weatherTable.push_back(processingData);
                        CityLastWeather[city_name].push_back(processingData);

                        break;

                    default:
                        processingData.max_min_temperature.first = std::max(static_cast<double>(processingData.max_min_temperature.first), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.max_min_temperature.second = std::min(static_cast<double>(processingData.max_min_temperature.second), static_cast<double>(jsonData["hourly"]["temperature_2m"][i]));
                        processingData.relative_humidity_2m += static_cast<int>(jsonData["hourly"]["relative_humidity_2m"][i]);
                        processingData.precipitation += static_cast<double>(jsonData["hourly"]["precipitation"][i]);
                        processingData.visibility += static_cast<double>(jsonData["hourly"]["visibility"][i]);
                        processingData.wind_speed_10m += static_cast<double>(jsonData["hourly"]["wind_speed_10m"][i]);
                        processingData.wind_direction_10m += static_cast<int>(jsonData["hourly"]["wind_direction_10m"][i]);
                }
            }
            return weatherTable;
        } else {
            throw std::runtime_error("HTTP request failed with status code: " + std::to_string(response.status_code));
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}
