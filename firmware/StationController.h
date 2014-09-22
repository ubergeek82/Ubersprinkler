#ifndef _STNCTL_H
#define _STNCTL_H

class StationController{
 
 public:
    StationController(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    int toggleStation(String arg);
    int toggleAllStations(String action);
    int getStatus();
  
  private:
    uint8_t STATION1;
    uint8_t STATION2;
    uint8_t STATION3;
    uint8_t STATION4;
};

#endif