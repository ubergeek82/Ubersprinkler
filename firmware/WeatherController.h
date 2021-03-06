#ifndef _WEATHERCTRL_H
#define _WEATHERCTRL_H

#include <application.h>
#include "HttpClient.h"

class WeatherController{
 
 public:
    bool update(int zipParam);
    bool wasRainYesterday();
    bool willRainToday();
    bool willRainTomorrow();

  protected:
  	http_request_t requestYesterday;
    http_request_t requestForecast;
  	int zipCode;
  	bool rainedYesterday;
  	bool rainToday;
  	bool rainTomorrow;

};

#endif