#include "WeatherUnderground.h"
#include "HttpClient.h"

WeatherUnderground::WeatherUnderground(){

}

/** Show temp in degree celsius */
void WeatherUnderground::setCelsius() {
	this->unitsForTemperature = "metric";
}
void WeatherUnderground::setFahrenheit() {
	this->unitsForTemperature = "imperial";
}

WeatherUnderground::Weather(String location, String apiKey) {
	this->location = location;
	this->client = client;
	this->apiKey = apiKey;

	// default:
	setFahrenheit();
}

void WeatherUnderground::update(int forecastDays, int historicalDays){
	this->forecastDays = forecastDays;
	this->historicalDays = historicalDays;

	weather_forecast_response_json forecastResp;
	weather_historical_response_json historicalResp;

	getForecast()
}

/**
 * Reads from the cache if there is a fresh and valid response.
 */
weather_forecast_response_json Weather::cachedForecast(int forecastDays, int historicalDays) {
	if (lastsync == 0 || (lastsync + weather_sync_interval) < millis()) {
		weather_forecast_response_json resp;
		lastReponse = resp;	
		lastReponse.isSuccess = false;
		if(this->updateForecast(lastReponse)){
			lastsync = millis();
		} else {
			//DO NOTHING
		}
	} else {
		Serial.println("using cached weather");
		lastReponse.cached = true;
	}
	return lastReponse;
}

bool Weather::getForecast(weather_forecast_response_json& response) {
	request.hostname = "api.openweathermap.org";
	request.port = 80;
	request.path = "/data/2.5/forecast/daily?q=" //
	+ location // e.g. "Berlin,de"
			+ "&units=" + unitsForTemperature // metric or imperial
			+ "&cnt=2" // number of days
			+ "&mode=json" // xml or json
			+ "&APPID=" + apiKey; // see http://openweathermap.org/appid
	request.body = "";

	http_response_t http_response;
	this->client->get(request, http_response);
	if (http_response.status == 200) {
		return parse(http_response.body, response);
	} else {
		Serial.print("weather request failed ");
		return false;
	}
}

bool Weather::getHistorical(weather_historical_response_json& response) {
	request.hostname = "api.openweathermap.org";
	request.port = 80;
	request.path = "/data/2.5/forecast/daily?q=" //
	+ location // e.g. "Berlin,de"
			+ "&units=" + unitsForTemperature // metric or imperial
			+ "&cnt=2" // number of days
			+ "&mode=json" // xml or json
			+ "&APPID=" + apiKey; // see http://openweathermap.org/appid
	request.body = "";

	http_response_t http_response;
	this->client->get(request, http_response);
	if (http_response.status == 200) {
		return parse(http_response.body, response);
	} else {
		Serial.print("weather request failed ");
		return false;
	}
}

bool WeatherUnderground::parse(String& data, weather_forecast_response_json response_today, weather_forecast_response_json response_tomorrow) {

	using namespace ArduinoJson::Parser;

	unsigned char buffer[data.length()];
	data.getBytes(buffer, sizeof(buffer), 0);
	JsonObject root = parser.parse((char*) buffer);
	if (!root.success()) {
		Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
		return false;
	}

	/* update for weatherunderground
	JsonValue daysList = root["list"];
	JsonValue today = daysList[0];
	JsonValue todays_weather = today["weather"][0];
	response_today.descr = todays_weather["description"];
	response_today.conditionCode = todays_weather["id"];
	
	JsonValue tomorrow = daysList[1];
	JsonValue tomorrows_weather = tomorrow["weather"][0];
	response_tomorrow.descr = tomorrows_weather["description"];
	response_tomorrow.conditionCode = tomorrows_weather["id"];
	*/
	
	response.isSuccess = true;
	return true;
}
