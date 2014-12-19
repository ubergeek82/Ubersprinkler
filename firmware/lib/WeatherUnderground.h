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
	int qpf_today_mm;
	int qpf_today_in;

	int qpf_tomorrow_mm;
	int qpf_tomorrow_in;
	
	bool isSuccess;
	// defaults:
	weather_forecast_response_t(): qpf_today_mm(-1), qpf_today_in(-1), qpf_tomorrow_mm(-1), qpf_tomorrow_in(-1), isSuccess(false) {};

} weather_forecast_response_t;

class WeatherUnderground : public WeatherController {
 
public:
	WeatherUnderground(HttpClient httpClient, int zipCode);
	bool update();
private:
	ArduinoJson::Parser::JsonParser<70> parser; // occupies 70 * 4 bytes
	weather_yesterday_response_t getYesterday(); 
	weather_forecast_response_t getForecast();
	weather_yesterday_response_t parseYesterday(String& data);
	weather_forecast_response_t parseForecast(String& data);
};

#endif