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
#define DEFAULT_CYCLE_TIME 60

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

char compiledDateTime[22] = COMPILED_DATE;
int stationStatus = 0;
int duration=10;
char* dateTime;
int signalStrength;
int cycleTimeSec = 600;
int currentStation = 1;

void setup() {
    if(debug){
        Serial.begin(9600);
    }

    strcat(compiledDateTime, " - ");
    strcat(compiledDateTime, COMPILED_TIME);
    
    
    Spark.variable("compiled", &compiledDateTime, STRING);
    Spark.variable("time", dateTime, STRING);
    Spark.variable("signal", &signalStrength, INT);
    Spark.variable("station", &stationStatus, INT);
    Spark.function("station", toggleStation);
    Spark.function("cycle", cycleAPI);

    Time.zone(-7);
    //Daily sync time with cloud @ 1AM
    Alarm.alarmRepeat(1,00,0, syncTime);

    // Schedule schedule(initializeDefaultSchedule());  
}

void syncTime(){
    Spark.syncTime();
    Serial.println("Alarm: - Syncing time with Spark Cloud");  

}

void loop() {
//    stationStatus=controller.getStatus();
//    Serial.println(stationStatus);

    dateTime = strcpy(dateTime, Time.hour() + ":" + Time.minute());
    // uint32_t  currentTime = Time.now();
    // String dateTime = rtc.ISODateUTCString(currentTime);

    signalStrength = WiFi.RSSI();
//    Serial.println(Time.timeStr());
    delay(1000);
}

int toggleStation(String arg){
    return controller.toggleStation(arg);
}

void loadProgram(){
    Alarm.alarmRepeat(dowTuesday,7,00,00,cycleAlarm); 
    Alarm.alarmRepeat(dowThursday,7,00,00,cycleAlarm); 
    Alarm.alarmRepeat(dowSaturday,7,00,00,cycleAlarm); 
}

void cycleAlarm(){
    //check manual/auto
    //check rain delay
    
    //turn on sprinklers
    runCycle(DEFAULT_CYCLE_TIME);
}

int cycleAPI(String args){
    cycleTimeSec = args.toInt();
    runCycle(cycleTimeSec);
    return 1;
}

void runCycle(int cycleTimeSec){
    for(int i=1; i<5;i++){
        String stationStr = String(i);
        controller.toggleStation(stationStr + ",on");
        delay(cycleTimeSec);
        controller.toggleStation("all,off");
    }
}