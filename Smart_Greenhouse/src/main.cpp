#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BH1750.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

// ==== Sensors ====
#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);
OneWire oneWire(PIN_ONEWIRE);
DallasTemperature ds18b20(&oneWire);
BH1750 lightMeter(BH1750_ADDRESS);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// ==== State ====
bool fanOn  = false;
bool pumpOn = false;
bool growOn = false;
uint32_t pumpStartMs = 0;
uint32_t lastSensorRead = 0;
uint32_t lastDisplayUpdate = 0;

// ==== Sensor status ====
bool dhtValid = false;
bool ds18b20Valid = false;
bool lightMeterValid = false;
bool lcdValid = false;

// ==== Last valid readings ====
float lastValidTemp = 0.0;
float lastValidHumidity = 0.0;
float lastValidSoilTemp = 0.0;
uint16_t lastValidLux = 0;
int lastValidSoil = 0;

// ==== Control functions ====
void setFan(bool on) { 
  if (fanOn != on) {
    fanOn = on; 
    digitalWrite(PIN_FAN, on ? HIGH : LOW);
    Serial.print(F("Fan: ")); Serial.println(on ? F("ON") : F("OFF"));
  }
}

void setPump(bool on) { 
  if (on && !pumpOn) {
    pumpStartMs = millis();
    Serial.println(F("Pump: START"));
  }
  if (!on && pumpOn) {
    Serial.println(F("Pump: STOP"));
  }
  pumpOn = on; 
  digitalWrite(PIN_PUMP, on ? HIGH : LOW); 
}

void setGrow(bool on) { 
  if (growOn != on) {
    growOn = on; 
    digitalWrite(PIN_GROW, on ? HIGH : LOW);
    Serial.print(F("Grow light: ")); Serial.println(on ? F("ON") : F("OFF"));
  }
}

// ==== Sensor reading functions ====
bool readDHT(float &temp, float &humidity) {
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  
  if (isnan(temp) || isnan(humidity)) {
    dhtValid = false;
    return false;
  }
  
  // Validate reasonable ranges
  if (temp < MIN_TEMP || temp > MAX_TEMP || humidity < MIN_HUMIDITY || humidity > MAX_HUMIDITY) {
    dhtValid = false;
    return false;
  }
  
  dhtValid = true;
  lastValidTemp = temp;
  lastValidHumidity = humidity;
  return true;
}

bool readDS18B20(float &temp) {
  ds18b20.requestTemperatures();
  temp = ds18b20.getTempCByIndex(0);
  
  if (temp == DEVICE_DISCONNECTED_C) {
    ds18b20Valid = false;
    return false;
  }
  
  // Validate reasonable range
  if (temp < MIN_SOIL_TEMP || temp > MAX_SOIL_TEMP) {
    ds18b20Valid = false;
    return false;
  }
  
  ds18b20Valid = true;
  lastValidSoilTemp = temp;
  return true;
}

bool readLightMeter(uint16_t &lux) {
  lux = lightMeter.readLightLevel();
  
  if (lux == 0xFFFF) { // Error reading
    lightMeterValid = false;
    return false;
  }
  
  lightMeterValid = true;
  lastValidLux = lux;
  return true;
}

bool readSoilMoisture(int &soilRaw) {
  soilRaw = analogRead(PIN_SOIL);
  
  // Validate reasonable range (0-1023 for 10-bit ADC)
  if (soilRaw < MIN_ADC_VALUE || soilRaw > MAX_ADC_VALUE) {
    return false;
  }
  
  lastValidSoil = soilRaw;
  return true;
}

// ==== Control logic functions ====
bool shouldTurnOnFan(float temp, float humidity) {
  // Turn on if temperature OR humidity is too high
  return (temp >= TEMP_ON_THRESHOLD || humidity >= HUMIDITY_ON_THRESHOLD);
}

bool shouldTurnOffFan(float temp, float humidity) {
  // Turn off only if BOTH temperature AND humidity are below thresholds
  return (temp <= TEMP_OFF_THRESHOLD && humidity <= HUMIDITY_OFF_THRESHOLD);
}

bool shouldTurnOnPump(int soilRaw) {
  return (soilRaw > 0 && soilRaw <= SOIL_DRY_THRESHOLD);
}

bool shouldTurnOffPump(int soilRaw) {
  return (soilRaw >= SOIL_WET_THRESHOLD);
}

bool shouldTurnOnGrowLight(uint16_t lux) {
  return (lux > 0 && lux <= LIGHT_LOW_THRESHOLD);
}

bool shouldTurnOffGrowLight(uint16_t lux) {
  return (lux >= LIGHT_HIGH_THRESHOLD);
}

// ==== Display functions ====
void updateDisplay(float temp, float humidity, int soilRaw, uint16_t lux) {
  if (millis() - lastDisplayUpdate < DISPLAY_UPDATE_INTERVAL_MS) return;
  lastDisplayUpdate = millis();
  
  if (!lcdValid) return;
  
  lcd.setCursor(0, 0);
  // Temperature display
  lcd.print(F("T:"));
  if (dhtValid) {
    lcd.print(temp, 1);
  } else {
    lcd.print(F("ERR"));
  }
  lcd.print(F("C "));
  
  // Humidity display
  lcd.print(F("H:"));
  if (dhtValid) {
    lcd.print(humidity, 0);
  } else {
    lcd.print(F("ERR"));
  }
  lcd.print(F("%"));
  
  lcd.setCursor(0, 1);
  // Soil moisture display
  lcd.print(F("S:"));
  lcd.print(soilRaw);
  lcd.print(F(" "));
  
  // Light display
  lcd.print(F("L:"));
  if (lightMeterValid) {
    lcd.print(lux);
  } else {
    lcd.print(F("ERR"));
  }
  lcd.print(F("    "));
}

void setupIO() {
  pinMode(PIN_FAN,  OUTPUT);
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_GROW, OUTPUT);
  pinMode(PIN_SOIL, INPUT);
  
  // Initialize all outputs to OFF
  setFan(false); 
  setPump(false); 
  setGrow(false);
}

void setupSensors() {
  // Initialize I2C
  Wire.begin();
  
  // Initialize DHT
  dht.begin();
  delay(1000); // DHT22 needs time to stabilize
  
  // Initialize DS18B20
  ds18b20.begin();
  
  // Initialize light meter
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("Light meter init failed"));
  }
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcdValid = true;
  
  lcd.setCursor(0, 0); 
  lcd.print(F("Smart Greenhouse"));
  lcd.setCursor(0, 1); 
  lcd.print(F("Init sensors..."));
  
  delay(2000);
  lcd.clear();
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println(F("=== Smart Greenhouse System ==="));
  
  setupIO();
  setupSensors();
  
  Serial.println(F("System initialized successfully"));
}

void loop() {
  uint32_t currentTime = millis();
  
  // Read sensors every 2 seconds (DHT22 requirement)
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL_MS) {
    lastSensorRead = currentTime;
    
    // Read all sensors
    float temp, humidity, soilTemp;
    uint16_t lux;
    int soilRaw;
    
    bool dhtOk = readDHT(temp, humidity);
    bool ds18b20Ok = readDS18B20(soilTemp);
    bool lightOk = readLightMeter(lux);
    bool soilOk = readSoilMoisture(soilRaw);
    
    // Use last valid readings if current readings failed
    if (!dhtOk) {
      temp = lastValidTemp;
      humidity = lastValidHumidity;
    }
    if (!ds18b20Ok) {
      soilTemp = lastValidSoilTemp;
    }
    if (!lightOk) {
      lux = lastValidLux;
    }
    if (!soilOk) {
      soilRaw = lastValidSoil;
    }
    
    // === Control Logic ===
    
    // Fan control with hysteresis
    if (dhtOk) {
      if (shouldTurnOnFan(temp, humidity)) {
        setFan(true);
      } else if (shouldTurnOffFan(temp, humidity)) {
        setFan(false);
      }
    }
    
    // Pump control with safety timer
    if (soilOk) {
      if (shouldTurnOnPump(soilRaw) && !pumpOn) {
        setPump(true);
             } else if (shouldTurnOffPump(soilRaw) || 
                  (pumpOn && (currentTime - pumpStartMs > PUMP_MAX_RUNTIME_MS))) {
        setPump(false);
      }
    }
    
    // Grow light control with hysteresis
    if (lightOk) {
      if (shouldTurnOnGrowLight(lux) && !growOn) {
        setGrow(true);
      } else if (shouldTurnOffGrowLight(lux) && growOn) {
        setGrow(false);
      }
    }
    
    // Update display
    updateDisplay(temp, humidity, soilRaw, lux);
    
    // Serial logging
    Serial.print(F("Tair=")); Serial.print(temp);
    Serial.print(F("C  RH=")); Serial.print(humidity);
    Serial.print(F("%  Tds=")); Serial.print(soilTemp);
    Serial.print(F("C  Soil=")); Serial.print(soilRaw);
    Serial.print(F("  Lux=")); Serial.print(lux);
    Serial.print(F(" | Fan=")); Serial.print(fanOn);
    Serial.print(F(" Pump=")); Serial.print(pumpOn);
    Serial.print(F(" Grow=")); Serial.println(growOn);
    
    // Status indicators
    if (!dhtOk) Serial.println(F("WARNING: DHT sensor error"));
    if (!ds18b20Ok) Serial.println(F("WARNING: DS18B20 sensor error"));
    if (!lightOk) Serial.println(F("WARNING: Light sensor error"));
    if (!soilOk) Serial.println(F("WARNING: Soil sensor error"));
  }
}
