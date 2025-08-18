#ifndef CONFIG_H
#define CONFIG_H

// ==== Pin Configuration ====
constexpr uint8_t PIN_DHT      = 4;         // AM2302/DHT22
constexpr uint8_t PIN_ONEWIRE  = 5;         // DS18B20 (optional)
constexpr uint8_t PIN_SOIL     = A0;        // YL-69 analog
constexpr uint8_t PIN_FAN      = 9;         // Fan (MOSFET/Relay)
constexpr uint8_t PIN_PUMP     = 10;        // Pump
constexpr uint8_t PIN_GROW     = 11;        // Grow LED

// ==== I2C Addresses ====
constexpr uint8_t LCD_ADDRESS  = 0x27;      // LCD I2C address (0x27 or 0x3F)
constexpr uint8_t BH1750_ADDRESS = 0x23;    // BH1750 address (0x23 or 0x5C)

// ==== Control Thresholds ====
// Temperature control (°C)
constexpr float TEMP_ON_THRESHOLD  = 30.0;  // Turn on fan when temp >= this
constexpr float TEMP_OFF_THRESHOLD = 28.0;  // Turn off fan when temp <= this

// Humidity control (%)
constexpr float HUMIDITY_ON_THRESHOLD  = 80.0;  // Turn on fan when humidity >= this
constexpr float HUMIDITY_OFF_THRESHOLD = 75.0;  // Turn off fan when humidity <= this

// Soil moisture control (ADC raw 0-1023)
constexpr int SOIL_DRY_THRESHOLD   = 450;   // Turn on pump when soil <= this (dry)
constexpr int SOIL_WET_THRESHOLD   = 650;   // Turn off pump when soil >= this (wet)

// Light control (lux)
constexpr uint16_t LIGHT_LOW_THRESHOLD  = 1500; // Turn on grow light when lux <= this
constexpr uint16_t LIGHT_HIGH_THRESHOLD = 2000; // Turn off grow light when lux >= this

// ==== Safety Limits ====
constexpr uint32_t PUMP_MAX_RUNTIME_MS = 30UL * 1000UL;  // 30 seconds max pump runtime
constexpr uint32_t SENSOR_READ_INTERVAL_MS = 2000UL;     // 2 seconds between sensor reads
constexpr uint32_t DISPLAY_UPDATE_INTERVAL_MS = 1000UL;  // 1 second display update

// ==== Sensor Validation Ranges ====
// Temperature ranges (°C)
constexpr float MIN_TEMP = -40.0;
constexpr float MAX_TEMP = 80.0;
constexpr float MIN_SOIL_TEMP = -55.0;
constexpr float MAX_SOIL_TEMP = 125.0;

// Humidity range (%)
constexpr float MIN_HUMIDITY = 0.0;
constexpr float MAX_HUMIDITY = 100.0;

// ADC range
constexpr int MIN_ADC_VALUE = 0;
constexpr int MAX_ADC_VALUE = 1023;

// ==== System Configuration ====
constexpr uint32_t SERIAL_BAUD_RATE = 115200;
constexpr uint8_t LCD_COLUMNS = 16;
constexpr uint8_t LCD_ROWS = 2;

// ==== Debug Options ====
constexpr bool ENABLE_DEBUG_PRINTS = true;  // Enable/disable debug messages
constexpr bool ENABLE_SENSOR_VALIDATION = true; // Enable sensor data validation

#endif // CONFIG_H
