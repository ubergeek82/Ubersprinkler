#ifndef _WCTRL_H
#define _WCTRL_H

class WeatherController{
 
 public:
    WeatherController();
    boolean rainCheck();
  
  private:
    uint8_t STATION1;
    uint8_t STATION2;
    uint8_t STATION3;
    uint8_t STATION4;
};

#endif