#ifndef _WEATHERUNDERGROUND_H
#define _WEATHERUNDERGROUND_H
#include "WeatherUnderground.h"
#include "JsonParser.h"


typedef struct weather_yesterday_response_t {
	int rain;
	int precipm;
	int precipi;

	bool isSuccess;
	// defaults:
	weather_yesterday_response_t(): rain(-1), precipi(-1), precipm(-1), isSuccess(false) {};

} weather_yesterday_response_t;

typedef struct weather_forecast_response_t {
	int today_mm;
	int today_in;

	int tomorrow_mm;
	int tomorrow_in;
	
	bool isSuccess;
	// defaults:
	weather_forecast_response_t(): today_mm(-1), today_in(-1), tomorrow_mm(-1), tomorrow_in(-1), isSuccess(false) {};

} weather_forecast_response_t;

class WeatherUnderground : public WeatherController {
 
public:
	WeatherUnderground();
	bool update(int zipParam);
private:
	ArduinoJson::Parser::JsonParser<70> parser; // occupies 70 * 4 bytes
	weather_yesterday_response_t getYesterday(int zipCode); 
	weather_forecast_response_t getForecast(int zipCode);
	weather_yesterday_response_t parseYesterday(String& data);
	weather_forecast_response_t parseForecast(String& data);
};

#endif