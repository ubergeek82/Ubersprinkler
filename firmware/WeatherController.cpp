#include <application.h>
#include "WeatherController.h"
#include "WeatherUnderground.h"

#define MIN_RAINFALL_M 0

WeatherController::WeatherController(int zipCode){
    this->zipCode = zipCode;
}

bool WeatherController::update() {
    weather_yesterday_response_t yesterday = this->getYesterday();
    if(yesterday.rain > 0 && yesterday.rprecipm > MIN_RAINFALL_M){
        this->rainedYesterday = true;
    }

    weather_forecast_response_t forecast = this->getForecast();
    if(forecast.qpf_today_mm > MIN_RAINFALL_M){
        this->rainToday = true;
    }

    weather_forecast_response_t forecast = this->getForecast();
    if(forecast.qpf_tomorrow_mm > MIN_RAINFALL_M){
        this->rainTomorrow = true;
    }

}

void Weather::getYesterday() {
    request.hostname = "api.wunderground.com";
    request.port = 80;
    request.path = "api/c91fc57e9677bd22/yesterday/q/" + zipCode + ".json";
    request.body = "";

    http_response_t http_response;
    this->client->get(request, http_response);
    if (http_response.status == 200) {
        return parseYesterday(http_response.body, response);
    } else {
        Serial.println("weather request failed");
        Serial.println(http_response.body);
        return false;
    }
}

void Weather::getForecast() {
    request.hostname = "api.wunderground.com";
    request.port = 80;
    request.path = "api/c91fc57e9677bd22/yesterday/q/" + zipCode + ".json";
    request.body = "";

    http_response_t http_response;
    this->client->get(request, http_response);
    if (http_response.status == 200) {
        return parseForecast(http_response.body, response);
    } else {
        Serial.print("weather request failed");
        Serial.println(http_response.body);
        return false;
    }
}

void WeatherUnderground::parseYesterday(String& data, weather_yesterday_response_t yesterday) {

    using namespace ArduinoJson::Parser;

    unsigned char buffer[data.length()];
    data.getBytes(buffer, sizeof(buffer), 0);
    JsonObject root = parser.parse((char*) buffer);
    if (!root.success()) {
        Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
        return false;
    }

    JsonValue history = root["history"];
    JsonValue dailySummary = history[0];
    yesterday.rprecipm= dailySummary["rain"];
    yesterday.rprecipm= dailySummary["rprecipm"];
    yesterday.rprecipm= dailySummary["rprecipi"];

    response.isSuccess = true;
    return true;
}

void WeatherUnderground::parseForecast(String& data, weather_forecast_response_t forecast) {

    using namespace ArduinoJson::Parser;

    unsigned char buffer[data.length()];
    data.getBytes(buffer, sizeof(buffer), 0);
    JsonObject root = parser.parse((char*) buffer);
    if (!root.success()) {
        Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
        return false;
    }

    JsonValue forecast = root["forecast"];
    JsonValue simpleForecast = history[0];
    JsonValue forecastDay = simpleForecast[0];
    JsonValue today = forecastDay[0];
    JsonValue tomorrow = forecastDay[1];

    today.qpf_allday_in = qpf_allday["in"];
    today.qpf_allday_mm = qpf_allday["mm"];

    today.qpf_allday_in = qpf_allday["in"];
    today.qpf_allday_in = qpf_allday["in"];

    response.isSuccess = true;
    return true;
}
