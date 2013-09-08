/**********************************************************************************************
 * Arduino RFtinyNet Library 
 * Version 0.1
 *
 * library in public domain
 **********************************************************************************************/
 
#include "RFtinyNet.h"

#include <RF24Network_config.h>
#include <RF24.h>
#include <RF24Network.h>

/* Constructor */
RFtinyNet::RFtinyNet(RF24& _radio): radio(_radio)
{
}

void RFtinyNet::begin(uint8_t _channel, uint8_t _node_id)
{
  node_ad = _node_id & 0x0F;
  net_ad  = (_node_id & 0xF0) >> 4;

  // type of node (every node with address = 0 is a gateway)
  gateway = (node_ad == 0);
  
  // setup radio
  radio.setChannel(_channel);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_16);
  radio.setPayloadSize(8);

  if (gateway) {
    // gateway node receive multicast and network unicast frame
    radio.openReadingPipe(1, 0xF0F0F0F000LL);
    radio.openReadingPipe(2, 0xF0F0F0F000LL + net_ad);
    radio.startListening();
  } else {
    // simple node
    // do nothing
  } 
  // Spew debugging state about the radio
  //radio.printDetails();
}

void RFtinyNet::update(void) {
  if (gateway) {
    // send hello message
    radio.openWritingPipe(0xF0F0F0F000LL);
    radio.powerUp();
    delay(100);
    uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8}; 
    bool write_ok = radio.write(&data, sizeof(data), 1);
  }
  // receive message
  // Dump the payloads until we've gotten everything
  byte buffer[8];
  bool done = false;
  // if there is data ready
  if (radio.available())
  {
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = radio.read(&buffer, sizeof(buffer));
      printf("\n\rGot payload (%d): ", sizeof(buffer));
      int i = 0;
      do
        printf("%d ", buffer[i]);
      while (++i < sizeof(buffer));
      printf("\n\r");
      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);
    }
  }
}