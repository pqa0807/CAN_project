// Include necessary libraries
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <mcp2515.h>

// Initialize RTC and CAN variables
RTC_DS3231 rtc; 
struct can_frame canMsg;
MCP2515 mcp2515(10);

// Setup function - runs once at the beginning
void setup() {
  // Wait until serial connection is established
  while (!Serial);
  // Begin serial communication at 115200 baud rate
  Serial.begin(115200);
  // Begin I2C communication
  Wire.begin(); 
  // Initialize RTC
  rtc.begin(); 
  // Begin SPI communication
  SPI.begin(); 
  // Reset MCP2515 module
  mcp2515.reset();
  // Set CAN bitrate to 500 kbps and enter normal mode
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ); 
  mcp2515.setNormalMode(); 
}

// Loop function - runs repeatedly
void loop() {
  // Get current date and time from RTC
  DateTime now = rtc.now(); 
  
  // Populate CAN frame with date and time data
  canMsg.can_id = 0x072; 
  canMsg.can_dlc = 8; 
  canMsg.data[0] = now.hour(); 
  canMsg.data[1] = now.minute(); 
  canMsg.data[2] = now.second(); 
  canMsg.data[3] = now.day(); 
  canMsg.data[4] = now.month(); 
  canMsg.data[5] = now.year() % 100;

  // Send CAN frame over the bus
  mcp2515.sendMessage(&canMsg); 
}
