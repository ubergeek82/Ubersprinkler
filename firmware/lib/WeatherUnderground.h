#ifndef _WEATHERUNDERGROUND_H
#define _WEATHERUNDERGROUND_H

#include "application.h"
#include "JsonParser.h"
#include "HttpClient.h"

typedef struct weather_forecast_response_json {
	//get variables from weatherunderground
	//rain
	//descr
	bool isSuccess;
	bool cached;
	// defaults:
	// weather_forecast_response_json(): temp_high(255), temp_low(255), conditionCode(-1), isSuccess(false) {};
} weather_forecast_response_json;

typedef struct weather_historical_response_json {
	//get variables from weatherunderground
	//rain
	//descr
	bool isSuccess;
	bool cached;
	// defaults:
	// weather_historical_response_json(): temp_high(255), temp_low(255), conditionCode(-1), isSuccess(false) {};
} weather_historical_response_json;


class WeatherUnderground {
public:
	WeatherUnderground(String location, HttpClient* client, String apiKey); 
	update(int forecastDays, int historicalDays);
	void setCelsius();
	void setFahrenheit();

private:
	ArduinoJson::Parser::JsonParser<70> parser; // occupies 70 * 4 bytes
	http_request_t request;
	String location;
	String apiKey;
	String unitsForTemperature;
	HttpClient* client;
	bool parse(String& json, weather_forecast_response_json& response);

	// cache:
	unsigned long weather_sync_interval;
	unsigned long lastsync;
	weather_forecast_response_t lastReponse;
};

#endif