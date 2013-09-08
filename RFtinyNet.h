#ifndef RFtinyNet_H
#define RFtinyNet_H

// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// define struct for routing table

class RF24;

class RFtinyNet
{
  public:
    RFtinyNet(RF24& _radio);
    void begin(uint8_t _channel, uint8_t _node_id);
    void update(void);
  protected:
    //boolean rxBuffer[59];
  private:
    RF24& radio;
    uint8_t node_ad;
    uint8_t net_ad;
    bool gateway;
    // add routing table  
};

#endif