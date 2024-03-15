// Include necessary libraries
#include <SPI.h>
#include <mcp2515.h>

// Define MCP2515 CS pin
#define CS_PIN 10

// Initialize MCP2515 object with CS pin
struct can_frame canMsg;
MCP2515 mcp2515(CS_PIN);

// Setup function - runs once at the beginning
void setup() {
  // Begin serial communication at 115200 baud rate
  Serial.begin(115200);
  
  // Reset MCP2515 module
  mcp2515.reset();
  
  // Set CAN bitrate to 500 kbps
  mcp2515.setBitrate(CAN_500KBPS);
  
  // Enter normal mode
  mcp2515.setNormalMode();
  
  // Print header for CAN message reading
  Serial.println("------- CAN Read ----------");
  Serial.println("ID DLC DATA");
}

// Loop function - runs repeatedly
void loop() {
  // Check if there's a message available to read
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    // Print CAN message ID, DLC, and data
    Serial.print(canMsg.can_id, HEX); 
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc, HEX); 
    Serial.print(" ");
    // Print each byte of data
    for (int i = 0; i < canMsg.can_dlc; i++) { 
      Serial.print(canMsg.data[i], HEX);
      Serial.print(" ");
    }
    // Move to the next line for the next message
    Serial.println(); 
  }
}
