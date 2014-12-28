#include <application.h>
#include <string.h>
#include "WeatherController.h"
#include "WeatherUnderground.h"
#include "Keys.h"
#include "HttpClient.h"

#define MIN_RAINFALL_MM 4

http_header_t http_headers[] = {
    //{ "Content-Type", "application/json" },
    { "Accept" , "application/json" },
    { "AppKey" , APIGEE_KEY},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

WeatherUnderground::WeatherUnderground(){
    //default constructor
}

bool WeatherUnderground::update(int zipParam){
    weather_yesterday_response_t yesterdaysWeather = getYesterday(zipParam);
    weather_forecast_response_t weatherForecast = getForecast(zipParam);

    //Set flag for Yesterday Rain
    if(yesterdaysWeather.rain > 0 && yesterdaysWeather.precipm > MIN_RAINFALL_MM){
        rainedYesterday = true;
    }else{
        rainedYesterday = false;
    }
    //DEBUG
    Serial.println("yesterdaysWeather.rain = " + yesterdaysWeather.rain);
    Serial.println("yesterdaysWeather.precipm = " + yesterdaysWeather.precipm);
    Serial.println("MIN_RAINFALL_MM = " + MIN_RAINFALL_MM);
    Serial.println("rainedYesterday = " + rainedYesterday);

    //Set flag for Today Rain
    if(weatherForecast.today_mm > MIN_RAINFALL_MM){
        rainToday = true;
    }else{
        rainToday = false;
    }
    Serial.println("weatherForecast.today_mm = " + weatherForecast.today_mm);
    Serial.println("MIN_RAINFALL_MM = " +MIN_RAINFALL_MM);
    Serial.println("rainToday = " + rainToday);

    //Set flag for Tomorrow Rain
    if(weatherForecast.tomorrow_mm > MIN_RAINFALL_MM){
        rainTomorrow = true;
    }else{
        rainTomorrow = false;
    }
    Serial.println("weatherForecast.tomorrow_mm = " + weatherForecast.tomorrow_mm);
    Serial.println("MIN_RAINFALL_MM = " + MIN_RAINFALL_MM);
    Serial.println("rainTomorrow = " + rainTomorrow);

    if(yesterdaysWeather.isSuccess && weatherForecast.isSuccess){
        Serial.println("Weather update successfull for both yesterday and forecast");
        return true;
    }else{
        Serial.println("Weather update failed!!!");
        return false;
    }
}

weather_yesterday_response_t WeatherUnderground::getYesterday(int zipCode) {
    requestYesterday.hostname = "ubergeek-prod.apigee.net";
    requestYesterday.port = 80;
    requestYesterday.path = "/weatherunderground/yesterday/q/" + String(zipCode) + ".json";
    requestYesterday.body = "";

    http_response_t http_response_yesterday;
    HttpClient client;

    client.get(requestYesterday, http_response_yesterday, http_headers);
    if (http_response_yesterday.status == 200) {
        return parseYesterday(http_response_yesterday.body);
    } else {
        Serial.println("weather requestYesterday failed");
        Serial.println(http_response_yesterday.body);
    }
}

weather_forecast_response_t WeatherUnderground::getForecast(int zipCode) {
    requestForecast.hostname = "ubergeek-prod.apigee.net";
    requestForecast.port = 80;
    requestForecast.path = "/weatherunderground/forecast/q/" + String(zipCode) + ".json";
    requestForecast.body = "";

    http_response_t http_response_forecast;
    HttpClient client;

    client.get(requestForecast, http_response_forecast, http_headers);
    if (http_response_forecast.status == 200) {
        return parseForecast(http_response_forecast.body);
    } else {
        Serial.print("weather requestForecast failed");
        Serial.println(http_response_forecast.body);
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
        yesterday.isSuccess = false;
    }

    JsonValue response = root["yesterday"];
    yesterday.rain = response["rain"];
    yesterday.precipi = response["precipi"];
    yesterday.precipm = response["precipm"];
    yesterday.isSuccess = true;

    Serial.println("yesterday.rain = " + String(yesterday.rain));
    Serial.println("yesterday.precipi = " + String(yesterday.precipi));
    Serial.println("yesterday.precipm = " + String(yesterday.precipm));
    Serial.println("Parsing successful");
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
        forecast.isSuccess=false;
    }

    JsonValue forecast_json = root["forecast"];
    JsonValue today = forecast_json[0];
    forecast.today_in  = today["today"]["in"];
    forecast.today_mm = today["today"]["mm"];

    JsonValue tomorrow = forecast_json[1];
    forecast.tomorrow_in = tomorrow["tomorrow"]["in"];
    forecast.tomorrow_mm = tomorrow["tomorrow"]["mm"];
    forecast.isSuccess=true;

    Serial.println("forecast.today_in = " + String(forecast.today_in));
    Serial.println("forecast.today_mm = " + String(forecast.today_mm));
    Serial.println("forecast.tomorrow_in = " + String(forecast.tomorrow_in));
    Serial.println("forecast.tomorrow_mm = " + String(forecast.tomorrow_mm));
    Serial.println("forecast.isSuccess = " + String(forecast.isSuccess));

    Serial.println("Parsing successful");
    return forecast;
}



