#ifndef _WEATHERCTRL_H
#define _WEATHERCTRL_H

#include <application.h>

typedef struct weather_yesterday_response_t {
	int rain;
	int rprecipm;
	int precipi;

	bool isSuccess;
	// defaults:
	weather_yesterday_response_t(): rain(-1), rprecipm(-1), isSuccess(false) {};

} weather_yesterday_response_t;

typedef struct weather_forecast_response_t {
	int qpf_today_mm;
	int qpf_today_in;

	int qpf_tomorrow_mm;
	int qpf_tomorrow_in;
	
	bool isSuccess;
	// defaults:
	weather_forecast_response_t(): qpf_today(-1), qpf_tomorrow(-1), isSuccess(false) {};

} weather_forecast_response_t;

class WeatherController{
 
 public:
    WeatherController(String apiKey, int zipCode);
    void update();
    bool rainedYesterday;
    bool rainToday;
    bool rainTomorrow;

  protected:
  	String apiKey;
  	int zipCode;
};

#endif