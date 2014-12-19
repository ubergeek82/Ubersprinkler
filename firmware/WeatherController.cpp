#include <application.h>
#include "WeatherController.h"
#include "WeatherUnderground.h"

bool WeatherController::wasRainYesterday(){
    return rainedYesterday;
}

bool WeatherController::willRainToday(){
    return rainToday;
}

bool WeatherController::willRainTomorrow(){
    return rainTomorrow;
}

