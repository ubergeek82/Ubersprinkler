#include <application.h>
#include "StationController.h"

#define ON LOW
#define OFF HIGH

StationController::StationController(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4){
    this->STATION1 = pin1;
    this->STATION3 = pin3;
    this->STATION2 = pin2;
    this->STATION4 = pin4;
}

int StationController::toggleStation(String arg){
    /*===============================
    Return Codes:
    101 = STATION1 ON
    102 = STATION2 ON
    103 = STATION3 ON
    104 = STATION4 ON
    
    91 = STATION1 OFF
    92 = STATION2 OFF
    93 = STATION3 OFF
    94 = STATION4 OFf
    ================================*/
    Serial.println(arg);
    int commaIndex = arg.indexOf(',');

    arg.toUpperCase();
    String firstArg = arg.substring(0,commaIndex);
    String secondArg = arg.substring(commaIndex+1);
    
    Serial.println(firstArg);
    Serial.println(secondArg);
    
    if (firstArg == "ALL"){
        return toggleAllStations(secondArg);
    }
    
    if (secondArg == "ON"){
        if (firstArg == "1"){
            digitalWrite(STATION1,ON);
            return 101;
        }else if (firstArg == "2" ){
            digitalWrite(STATION2,ON);
            return 102;
        }else if (firstArg == "3" ){
            digitalWrite(STATION3,ON);
            return 103;
        }else if (firstArg == "4"){
            digitalWrite(STATION4,ON);
            return 104;
        }
    }else if(secondArg == "OFF"){
        if (firstArg == "1"){
            digitalWrite(STATION1,OFF);
            return 91;
        }else if (firstArg == "2" ){
            digitalWrite(STATION2,OFF);
            return 92;
        }else if (firstArg == "3" ){
            digitalWrite(STATION3,OFF);
            return 93;
        }else if (firstArg == "4"){
            digitalWrite(STATION4,OFF);
            return 94;
        }
    }
    return -1;
}

int StationController::toggleAllStations(String action){
    if (action == "ON"){
        digitalWrite(STATION1,ON);
        digitalWrite(STATION2,ON);
        digitalWrite(STATION3,ON);
        digitalWrite(STATION4,ON);
        return 100;
    }
    if(action == "OFF"){
        digitalWrite(STATION1,OFF);
        digitalWrite(STATION2,OFF);
        digitalWrite(STATION3,OFF);
        digitalWrite(STATION4,OFF);
        return 99;
    }
    
    return -1;
}


int StationController::getStatus(){
    /*===============================
    Status Codes:
    0  = 0 0 0 0
    1  = 0 0 0 1
    2  = 0 0 1 0
    3  = 0 0 1 1
    4  = 0 1 0 0
    5  = 0 1 0 1
    6  = 0 1 1 0
    7  = 0 1 1 1
    8  = 1 0 0 0
    9  = 1 0 0 1
    10 = 1 0 1 0
    11 = 1 0 1 1
    12 = 1 1 0 0
    13 = 1 1 0 1
    14 = 1 1 1 0
    15 = 1 1 1 1
    ================================*/
    return
	    ((digitalRead(STATION1) == OFF ? 0 : 1)<<3) +
		((digitalRead(STATION2) == OFF ? 0 : 1)<<2) +
		((digitalRead(STATION3) == OFF ? 0 : 1)<<1) +
		((digitalRead(STATION4) == OFF ? 0 : 1));
}
