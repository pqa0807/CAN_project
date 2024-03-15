// Include necessary libraries
#include <SPI.h> 
#include <mcp2515.h> 
#include <DHT.h> 

// Define pin for DHT sensor and sensor type
#define DHTPIN 8
#define DHTTYPE DHT11

// Declare a structure to hold CAN frame data
struct can_frame canMsg;

// Initialize MCP2515 object with CS pin 10
MCP2515 mcp2515(10);

// Initialize DHT sensor object
DHT dht(DHTPIN, DHTTYPE); 

// Setup function - runs once at the beginning
void setup() {
  // Wait until serial connection is established
  while (!Serial);
  // Begin serial communication at 115200 baud rate
  Serial.begin(115200);
  // Begin SPI communication
  SPI.begin(); 
  // Initialize DHT sensor
  dht.begin(); 
  // Reset MCP2515 module
  mcp2515.reset();
  // Set CAN bitrate to 500 kbps and enter normal mode
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ); 
  mcp2515.setNormalMode();
}

// Loop function - runs repeatedly
void loop() {
  // Read humidity from DHT sensor
  int h = dht.readHumidity(); 
  // Read temperature from DHT sensor
  int t = dht.readTemperature(); 
  
  // Populate CAN frame with humidity and temperature data
  canMsg.can_id = 0x036; 
  canMsg.can_dlc = 8; 
  canMsg.data[6] = h; 
  canMsg.data[7] = t; 
  
  // Send CAN frame over the bus
  mcp2515.sendMessage(&canMsg); 
  
  // Delay for 1 second
  delay(1000);
}
