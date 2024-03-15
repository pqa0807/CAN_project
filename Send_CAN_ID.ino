// Include necessary libraries
#include <SPI.h>
#include <mcp_can.h>

// Define CS pin for MCP2515
const int SPI_CS_PIN = 10;

// Initialize MCP_CAN object with CS pin
MCP_CAN CAN(SPI_CS_PIN);

// Setup function - runs once at the beginning
void setup() {
  // Begin serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // Wait for Serial Monitor to open
  while (!Serial) {}
  
  // Begin SPI communication
  SPI.begin();
  
  // Initialize MCP2515 module
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) { 
    Serial.println("MCP2515 Initialized Successfully!");
    // Set MCP2515 to normal mode
    CAN.setMode(MCP_NORMAL); 
  } else {
    Serial.println("Error Initializing MCP2515...");
  }
}

// Loop function - runs repeatedly
void loop() {
  // Data to be sent
  unsigned char data1[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  // CAN ID for message
  unsigned short canId1 = 0x4000052D;
  
  // Send the message
  byte sndStat1 = CAN.sendMsgBuf(canId1, 0, 8, data1);
  
  // Check if message was sent successfully
  if (sndStat1 == CAN_OK) {
    Serial.println("Message Sent Successfully - CAN ID 0x4000052D!");
  } else {
    Serial.println("Error Sending Message - CAN ID 0x4000052D...");
  }
  
  // Delay for 1 second before sending the next message
  delay(1000); 
}
