#include <Arduino.h>
#include "disk91_LoRaE5.h"

Disk91_LoRaE5 lorae5(false); // true, false whatever

uint8_t deveui[] = { 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEE };
uint8_t appeui[] = { 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAD };
uint8_t appkey[] = { 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43 };

#define RXD2 13
#define TXD2 12

void setup() {

  Serial.begin(9600);
  uint32_t start = millis();
  while ( !Serial && (millis() - start) < 1500 );  // Open the Serial Monitor to get started or wait for 1.5"

  // init the library, search the LORAE5 over the different WIO port available
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2, false);
  if ( ! lorae5.begin(&Serial1) ) {
    Serial.println("LoRa E5 Init Failed");
    while(1); 
  }

  // Setup the LoRaWan Credentials
  if ( ! lorae5.setup(
          DSKLORAE5_ZONE_EU868,     // LoRaWan Radio Zone EU868 here
          deveui,
          appeui,
          appkey
       ) ){
    Serial.println("LoRa E5 Setup Failed");
    while(1);         
  }

}

void loop() {
  static uint8_t data[] = { 0x01, 0x02, 0x03, 0x04 }; 

  // Send an uplink message. The Join is automatically performed
  if ( lorae5.send_sync(
        1,              // LoRaWan Port
        data,           // data array
        sizeof(data),   // size of the data
        false,          // we are not expecting a ack
        7,              // Spread Factor
        14              // Tx Power in dBm
       ) 
  ) {
      Serial.println("Uplink done");
      if ( lorae5.isDownlinkReceived() ) {
        Serial.println("A downlink has been received");
        if ( lorae5.isDownlinkPending() ) {
          Serial.println("More downlink are pending");
        }
      }
  }
  delay(30000);
    
}