// This #include statement was automatically added by the Spark IDE.
//#include "SparkTime/SparkTime.h"

// This #include statement was automatically added by the Spark IDE.
#include "StationController.h"

// This #include statement was automatically added by the Spark IDE.
#include "TimeAlarms.h"

#undef now()

#include <application.h>

#define COMPILED_TIME __TIME__
#define COMPILED_DATE __DATE__

#define ON LOW
#define OFF HIGH

const uint8_t STATION1=D0;
const uint8_t STATION2=D1;
const uint8_t STATION3=D3;
const uint8_t STATION4=D4;

const bool debug = true;

SYSTEM_MODE(AUTOMATIC); //SEMIAUTOMATIC = no cloud conection by default, need to call Spark.connect();

class RunBeforeSetup {
public:
	RunBeforeSetup() {
	    pinMode(STATION1, OUTPUT);
	    pinMode(STATION2, OUTPUT);
	    pinMode(STATION3, OUTPUT);
	    pinMode(STATION4, OUTPUT);
	    
	    digitalWrite(STATION1, OFF);
	    digitalWrite(STATION2, OFF);
	    digitalWrite(STATION3, OFF);
	    digitalWrite(STATION4, OFF);
	}
};

RunBeforeSetup runBeforeSetup;

StationController controller(STATION1, STATION2, STATION3, STATION4);

int ALARM_CYCLE_TIME = 600;
char compiledDateTime[22] = COMPILED_DATE;
int stationStatus = 0;
int duration=10;
int signalStrength;
int cycleTimeSec = 600;
int currentStation;
uint8_t isRunning = false;
unsigned long int runTimer;

void setup() {
    if(debug){
        Serial.begin(9600);
    }
	
	Serial.println("Setting up UberCore");
    strcat(compiledDateTime, " - ");
    strcat(compiledDateTime, COMPILED_TIME);
    
    Spark.variable("compiled", &compiledDateTime, STRING);
    Spark.variable("signal", &signalStrength, INT);
    Spark.variable("station", &stationStatus, INT);
    Spark.function("station", toggleStation);
    Spark.function("cycle", cycleAPI);
	Spark.function("alarmTime",setAlarmTime);
	Spark.function("time", setTime);

    Time.zone(-7);
    //Daily sync time with cloud @ 1AM
    Alarm.alarmRepeat(1,00,0, syncTime);

	Alarm.alarmRepeat(dowTuesday,7,00,00,cycleAlarm); 
    Alarm.alarmRepeat(dowThursday,7,00,00,cycleAlarm); 
    Alarm.alarmRepeat(dowSaturday,7,00,00,cycleAlarm);
}

void syncTime(){
    Spark.syncTime();
    Serial.println("Alarm: - Syncing time with Spark Cloud");  
}

int setTime(String arg){
	Serial.println("setTime(): " + arg + " seconds");
	Time.setTime(arg.toInt()); // set time to Tuesday 6:59:00am Sept 23 2014, 1411480790
	return 1;
}

void loop() {
    stationStatus=controller.getStatus();
    Serial.println("stationStatus = " + String(stationStatus));
    signalStrength = WiFi.RSSI();
    Serial.println(Time.timeStr());
   
	if(isRunning){
		Serial.println("isRunning! time left = " + (String(runTimer-Time.now())));
		if(Time.now() > runTimer){
			Serial.println("Turning off sprinklers");
			controller.toggleStation("all,off");
			if(currentStation < 4 && currentStation > 0){
				currentStation++;
				runTimer = Time.now() + cycleTimeSec;
				controller.toggleStation(String(currentStation) + ",on");
			}else{ //last station reached
				Serial.println("Last Station reached, all sprinklers off");
				currentStation = 1;
				isRunning = false;
			}
		}
	}
	
	Alarm.delay(1000);
}

int toggleStation(String arg){
    return controller.toggleStation(arg);
}

void cycleAlarm(){
	Serial.println("cycleAlarm()");
    //check manual/auto
    //check rain delay
    
    //turn on sprinklers
    startCycle(ALARM_CYCLE_TIME);
}

int cycleAPI(String arg){
	Serial.println("cycleAPI(): " + arg + " seconds");
    cycleTimeSec = arg.toInt();
    if(cycleTimeSec > 0){
		startCycle(cycleTimeSec);
		return 1;
	}else{
		cancelCycle();
		return 0;
	}
}

int setAlarmTime(String arg){
	Serial.println("setAlarmTime(): " + arg + " seconds");
    ALARM_CYCLE_TIME = arg.toInt();
	return 1;
}

void startCycle(int cycleTimeSec){
    Serial.println("startCycle() - Station1 ON");
	isRunning = true;
	runTimer = Time.now() + cycleTimeSec;
	Serial.println(Time.now());
	Serial.println(runTimer);
	controller.toggleStation("1,on");
	currentStation = 1;
}

void runCycle(){

}

void cancelCycle(){
    Serial.println("cancelCycle()");
	controller.toggleStation("all,off");
	isRunning = false;
	currentStation = 0;
}