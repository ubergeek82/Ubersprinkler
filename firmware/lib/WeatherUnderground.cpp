#include <application.h>
#include "WeatherController.h"
#include "WeatherUnderground.h"

#define MIN_RAINFALL_M 0

WeatherUnderground::WeatherUnderground(String apiKey, int zipCode) : WeatherController(apiKey, zipCode){

}

void WeatherUnderground::getYesterday() {
    request.hostname = "api.wunderground.com";
    request.port = 80;
    request.path = "api/" + apiKey + "/yesterday/q/" + zipCode + ".json";
    request.body = "";

    http_response_t http_response;
    this->client->get(request, http_response);
    if (http_response.status == 200) {
        parseYesterday(http_response.body, response);
    } else {
        Serial.println("weather request failed");
        Serial.println(http_response.body);
    }
}

void WeatherUnderground::getForecast() {
    request.hostname = "api.wunderground.com";
    request.port = 80;
    request.path = "api/" + apiKey + "/yesterday/q/" + zipCode + ".json";
    request.body = "";

    http_response_t http_response;
    this->client->get(request, http_response);
    if (http_response.status == 200) {
        parseForecast(http_response.body, response);
    } else {
        Serial.print("weather request failed");
        Serial.println(http_response.body);
    }
}

void WeatherUnderground::parseYesterday(String& data, weather_yesterday_response_t yesterday) {

    using namespace ArduinoJson::Parser;

    unsigned char buffer[data.length()];
    data.getBytes(buffer, sizeof(buffer), 0);
    JsonObject root = parser.parse((char*) buffer);
    if (!root.success()) {
        Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
    }

    JsonValue history = root["history"];
    JsonValue dailySummary = history[0];
    yesterday.rprecipm= dailySummary["rain"];
    yesterday.rprecipm= dailySummary["rprecipm"];
    yesterday.rprecipm= dailySummary["rprecipi"];
}

void WeatherUnderground::parseForecast(String& data, weather_forecast_response_t forecast) {

    using namespace ArduinoJson::Parser;

    unsigned char buffer[data.length()];
    data.getBytes(buffer, sizeof(buffer), 0);
    JsonObject root = parser.parse((char*) buffer);
    if (!root.success()) {
        Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
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
}
