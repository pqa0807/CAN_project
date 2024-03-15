// Include necessary libraries
#include <SPI.h>
#include <mcp2515.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>

// Define constants
#define CS_PIN 10
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
#define DHTPIN 8
#define DHTTYPE DHT11

// Initialize objects
struct can_frame canMsg;
MCP2515 mcp2515(CS_PIN);
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;

// Setup function - runs once at the beginning
void setup() {
  // Wait until serial connection is established
  while (!Serial);
  // Begin serial communication at 115200 baud rate
  Serial.begin(115200);
  // Begin SPI communication
  SPI.begin();
  // Initialize LCD
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CANBUS TUTORIAL");
  delay(3000);
  lcd.clear();
  // Reset MCP2515 module
  mcp2515.reset();
  // Set CAN bitrate to 500 kbps and enter normal mode
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
  // Initialize RTC
  rtc.begin();
}

// Loop function - runs repeatedly
void loop() {
  // Receive and process CAN messages
  receiveMessages();
  // Display data on LCD
  displayData();
}

// Function to receive CAN messages
void receiveMessages() {
  while (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if (canMsg.can_id == 0x036) {
      // Data received from DHT11 sensor
      int humidity = canMsg.data[6];
      int temperature = canMsg.data[7];
      Serial.print("Received Data from DHT11: Humidity=");
      Serial.print(humidity);
      Serial.print(" Temperature=");
      Serial.println(temperature);
      // Display humidity and temperature on LCD
      lcd.setCursor(0, 0);
      lcd.print("Humi:");
      lcd.print(humidity);
      lcd.setCursor(9, 0);
      lcd.print("Temp:");
      lcd.print(temperature);
    } else if (canMsg.can_id == 0x072) {
      // Data received from DS3231 RTC
      int hour = canMsg.data[0];
      int minute = canMsg.data[1];
      int second = canMsg.data[2];
      Serial.print("Received Data from DS3231: ");
      Serial.print(hour);
      Serial.print(":");
      Serial.print(minute);
      Serial.print(":");
      Serial.println(second);
      // Display time on LCD
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print(hour);
      lcd.print(":");
      lcd.print(minute);
      lcd.print(":");
      lcd.print(second);
    }
  }
}
