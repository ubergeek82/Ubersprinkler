#include "openweathermap.h"

/** Show temp in degree celsius */
void Weather::setCelsius() {
	this->unitsForTemperature = "metric";
}
void Weather::setFahrenheit() {
	this->unitsForTemperature = "imperial";
}

Weather::Weather(String location, HttpClient* client, String apiKey) {
	this->location = location;
	this->client = client;
	this->apiKey = apiKey;

	// default:
	setCelsius();

	// init cache
	this->lastsync = 0;
	this->weather_sync_interval = 1000 * 3600 * 8; // milliseconds

}


/**
 * Reads from the cache if there is a fresh and valid response.
 */
weather_forecast_response_t Weather::cachedUpdate() {
	if (lastsync == 0 || (lastsync + weather_sync_interval) < millis()) {
		weather_forecast_response_t resp;
		lastReponse = resp;	
		lastReponse.isSuccess = false;
		if(this->updateForecast(lastReponse)){
			lastsync = millis();
		} else {

		}
	} else {
		Serial.println("using cached weather");
		lastReponse.cached = true;
	}
	return lastReponse;
}


bool Weather::updateForecast(weather_forecast_response_t& response) {
	request.forceIp = true;
	request.ip = IPAddress(188, 226, 224, 148);
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


/**
 *
 */
bool Weather::parse(String& data, weather_forecast_response_t response_today, weather_forecast_response_t response_tomorrow) {
	/*
	 * example 2 day forecast for glendale:
	 * http://api.openweathermap.org/data/2.5/forecast/daily?q=glendale&cnt=2&units=imperial&mode=json
	 *	{
	 *		"cod": "200",
	 *		"message": 0.1876,
	 *		"city": {
	 *			"id": 5352423,
	 *			"name": "Glendale",
	 *			"coord": {
	 *				"lon": -118.255081,
	 *				"lat": 34.142509
	 *			},
	 *			"country": "US",
	 *			"population": 0,
	 *			"sys": {
	 *				"population": 0
	 *			}
	 *		},
	 *		"cnt": 2,
	 *		"list": [
	 *			{
	 *				"dt": 1417460400,
	 *				"temp": {
	 *					"day": 66.96,
	 *					"min": 51.28,
	 *					"max": 66.96,
	 *					"night": 51.48,
	 *					"eve": 64.56,
	 *					"morn": 66.96
	 *				},
	 *				"pressure": 930.67,
	 *				"humidity": 53,
	 *				"weather": [
	 *					{
	 *						"id": 803,
	 *						"main": "Clouds",
	 *						"description": "broken clouds",
	 *						"icon": "04d"
	 *					}
	 *				],
	 *				"speed": 1.23,
	 *				"deg": 111,
	 *				"clouds": 64
	 *			},
	 *			{
	 *				"dt": 1417546800,
	 *				"temp": {
	 *					"day": 53.29,
	 *					"min": 53.06,
	 *					"max": 55.76,
	 *					"night": 55.76,
	 *					"eve": 55.11,
	 *					"morn": 53.06
	 *				},
	 *				"pressure": 926.44,
	 *				"humidity": 93,
	 *				"weather": [
	 *					{
	 *						"id": 503,
	 *						"main": "Rain",
	 *						"description": "very heavy rain",
	 *						"icon": "10d"
	 *					}
	 *				],
	 *				"speed": 2.76,
	 *				"deg": 67,
	 *				"clouds": 92,
	 *				"rain": 67
	 *			}
	 *		]
	 *	}
	 */
	using namespace ArduinoJson::Parser;

	unsigned char buffer[data.length()];
	data.getBytes(buffer, sizeof(buffer), 0);
	JsonObject root = parser.parse((char*) buffer);
	if (!root.success()) {
		Serial.println("Parsing fail: could be an invalid JSON, or too many tokens");
		return false;
	}

	JsonValue daysList = root["list"];
	JsonValue today = daysList[0];
	JsonValue todays_weather = today["weather"][0];
	response_today.descr = todays_weather["description"];
	response_today.conditionCode = todays_weather["id"];
	
	JsonValue tomorrow = daysList[1];
	JsonValue tomorrows_weather = tomorrow["weather"][0];
	response_tomorrow.descr = tomorrows_weather["description"];
	response_tomorrow.conditionCode = tomorrows_weather["id"];
	
	response.isSuccess = true;
	return true;
}
