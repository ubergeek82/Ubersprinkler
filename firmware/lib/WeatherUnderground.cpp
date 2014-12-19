#include <application.h>
#include <string.h>
#include "WeatherController.h"
#include "WeatherUnderground.h"
#include "Keys.h"

#define MIN_RAINFALL_MM 4

WeatherUnderground::WeatherUnderground(HttpClient httpClient, int zip){
    client = httpClient;
    zipCode = zip;
}

bool WeatherUnderground::update(){
    weather_yesterday_response_t yesterdaysWeather = getYesterday();
    weather_forecast_response_t weatherForecast = getForecast();

    //Set flag for Yesterday Rain
    if(yesterdaysWeather.rain > 0 && yesterdaysWeather.precipm > MIN_RAINFALL_MM){
        rainedYesterday = true;
    }else{
        rainedYesterday = false;
    }

    //Set flag for Today Rain
    if(weatherForecast.qpf_today_mm > MIN_RAINFALL_MM){
        rainToday = true;
    }else{
        rainToday = false;
    }

    //Set flag for Tomorrow Rain
    if(weatherForecast.qpf_tomorrow_mm > MIN_RAINFALL_MM){
        rainTomorrow = true;
    }else{
        rainTomorrow = false;
    }
}

weather_yesterday_response_t WeatherUnderground::getYesterday() {
    requestYesterday.hostname = "api.wunderground.com";
    requestYesterday.port = 80;
    requestYesterday.path = "api/" + WUNDERGRND_KEY + "/yesterday/q/" + zipCode + ".json";
    requestYesterday.body = "";

    http_response_t http_response;
    client.get(requestYesterday, http_response);
    if (http_response.status == 200) {
        return parseYesterday(http_response.body);
    } else {
        Serial.println("weather requestYesterday failed");
        Serial.println(http_response.body);
    }


}

weather_forecast_response_t WeatherUnderground::getForecast() {
    requestForecast.hostname = "api.wunderground.com";
    requestForecast.port = 80;
    requestForecast.path = "api/" + WUNDERGRND_KEY + "/yesterday/q/" + zipCode + ".json";
    requestForecast.body = "";

    http_response_t http_response;
    client.get(requestForecast, http_response);
    if (http_response.status == 200) {
        return parseForecast(http_response.body);
    } else {
        Serial.print("weather requestForecast failed");
        Serial.println(http_response.body);
    }
}

weather_yesterday_response_t WeatherUnderground::parseYesterday(String& data) {
    using namespace ArduinoJson::Parser;
    weather_yesterday_response_t yesterday;

    unsigned char buffer[data.length()];
    data.getBytes(buffer, sizeof(buffer), 0);
    JsonObject root = parser.parse((char*) buffer);

    if (!root.success()) {
        Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
    }

    JsonValue history = root["history"];
    JsonValue dailySummary = history[3];
    JsonValue summary = dailySummary[0];
    yesterday.rain = summary["rain"];
    yesterday.precipi = summary["precipi"];
    yesterday.precipm = summary["precipm"];

    return yesterday;
}

weather_forecast_response_t WeatherUnderground::parseForecast(String& data) {
    using namespace ArduinoJson::Parser;
    weather_forecast_response_t forecast;

    unsigned char buffer[data.length()];
    data.getBytes(buffer, sizeof(buffer), 0);
    JsonObject root = parser.parse((char*) buffer);

    if (!root.success()) {
        Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
    }

    JsonValue forecast_json = root["forecast"];
    JsonValue simpleForecast = forecast_json[1];
    JsonValue forecastDay = simpleForecast[0];

    JsonValue today = forecastDay[0];
    JsonValue today_qpf_allday = today["qpf_allday"];
    forecast.qpf_today_in  = today_qpf_allday["in"];
    forecast.qpf_today_mm = today_qpf_allday["mm"];

    JsonValue tomorrow = forecastDay[1];
    JsonValue tomorrow_qpf_allday = tomorrow["qpf_allday"];
    forecast.qpf_tomorrow_in = tomorrow_qpf_allday["in"];
    forecast.qpf_tomorrow_mm = tomorrow_qpf_allday["mm"];

    return forecast;
}



