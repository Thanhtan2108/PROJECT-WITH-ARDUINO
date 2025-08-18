#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Pin definitions for left motor driver (L298N)
const int motorKiriIN1 = 2;   // IN1 for left motor
const int motorKiriIN2 = 3;   // IN2 for left motor
const int motorKiriEN = 9;    // ENA (PWM) for left motor

// Pin definitions for right motor driver (L298N)
const int motorKananIN1 = 4;  // IN1 for right motor
const int motorKananIN2 = 5;  // IN2 for right motor
const int motorKananEN = 10;  // ENB (PWM) for right motor

// Emergency stop button pin (add physical button)
const int emergencyStopPin = 8;

// Safety and control parameters
const int thresholdMaju = 10;     // Forward tilt threshold
const int thresholdMundur = -10;  // Backward tilt threshold
const int thresholdBelok = 10;    // Left/Right tilt threshold
const int maxSpeed = 150;         // Maximum motor speed
const int minSpeed = 50;          // Minimum motor speed
const int speedRampStep = 10;     // Speed change per cycle
const unsigned long timeoutMs = 1000;  // Stop if no valid signal for 1 second
const unsigned long emergencyTimeoutMs = 5000; // Emergency stop after 5 seconds

// MPU6050 object
Adafruit_MPU6050 mpu;

// Global variables for safety and control
unsigned long lastValidSignal = 0;
unsigned long lastEmergencyCheck = 0;
int currentSpeed = 0;
bool emergencyStopActive = false;
bool sensorError = false;

// Calibration variables
float pitchOffset = 0;
float rollOffset = 0;
bool isCalibrated = false;

// Function to move forward with speed ramping
void maju(int targetSpeed) {
  if (currentSpeed < targetSpeed) {
    currentSpeed = min(currentSpeed + speedRampStep, targetSpeed);
  }
  
  digitalWrite(motorKiriIN1, HIGH);
  digitalWrite(motorKiriIN2, LOW);
  analogWrite(motorKiriEN, currentSpeed);

  digitalWrite(motorKananIN1, HIGH);
  digitalWrite(motorKananIN2, LOW);
  analogWrite(motorKananEN, currentSpeed);
}

// Function to move backward with speed ramping
void mundur(int targetSpeed) {
  if (currentSpeed < targetSpeed) {
    currentSpeed = min(currentSpeed + speedRampStep, targetSpeed);
  }
  
  digitalWrite(motorKiriIN1, LOW);
  digitalWrite(motorKiriIN2, HIGH);
  analogWrite(motorKiriEN, currentSpeed);

  digitalWrite(motorKananIN1, LOW);
  digitalWrite(motorKananIN2, HIGH);
  analogWrite(motorKananEN, currentSpeed);
}

// Function to turn left with speed ramping
void belokKiri(int targetSpeed) {
  if (currentSpeed < targetSpeed) {
    currentSpeed = min(currentSpeed + speedRampStep, targetSpeed);
  }
  
  digitalWrite(motorKiriIN1, LOW);
  digitalWrite(motorKiriIN2, HIGH);
  analogWrite(motorKiriEN, currentSpeed);

  digitalWrite(motorKananIN1, HIGH);
  digitalWrite(motorKananIN2, LOW);
  analogWrite(motorKananEN, currentSpeed);
}

// Function to turn right with speed ramping
void belokKanan(int targetSpeed) {
  if (currentSpeed < targetSpeed) {
    currentSpeed = min(currentSpeed + speedRampStep, targetSpeed);
  }
  
  digitalWrite(motorKiriIN1, HIGH);
  digitalWrite(motorKiriIN2, LOW);
  analogWrite(motorKiriEN, currentSpeed);

  digitalWrite(motorKananIN1, LOW);
  digitalWrite(motorKananIN2, HIGH);
  analogWrite(motorKananEN, currentSpeed);
}

// Function to stop with speed ramping
void berhenti() {
  if (currentSpeed > 0) {
    currentSpeed = max(currentSpeed - speedRampStep, 0);
  }
  
  digitalWrite(motorKiriIN1, LOW);
  digitalWrite(motorKiriIN2, LOW);
  analogWrite(motorKiriEN, currentSpeed);

  digitalWrite(motorKananIN1, LOW);
  digitalWrite(motorKananIN2, LOW);
  analogWrite(motorKananEN, currentSpeed);
}

// Emergency stop function
void emergencyStop() {
  currentSpeed = 0;
  digitalWrite(motorKiriIN1, LOW);
  digitalWrite(motorKiriIN2, LOW);
  analogWrite(motorKiriEN, 0);

  digitalWrite(motorKananIN1, LOW);
  digitalWrite(motorKananIN2, LOW);
  analogWrite(motorKananEN, 0);
  
  Serial.println("EMERGENCY STOP ACTIVATED!");
}

// Calibration function
void calibrateSensor() {
  Serial.println("Calibrating sensor... Keep the device level and still.");
  delay(2000);
  
  float pitchSum = 0;
  float rollSum = 0;
  const int calibrationSamples = 100;
  
  for (int i = 0; i < calibrationSamples; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    float pitch = atan2(a.acceleration.y, a.acceleration.z) * (180.0 / PI);
    float roll = atan2(a.acceleration.x, a.acceleration.z) * (180.0 / PI);
    
    pitchSum += pitch;
    rollSum += roll;
    delay(10);
  }
  
  pitchOffset = pitchSum / calibrationSamples;
  rollOffset = rollSum / calibrationSamples;
  isCalibrated = true;
  
  Serial.print("Calibration complete. Pitch offset: ");
  Serial.print(pitchOffset);
  Serial.print(" Roll offset: ");
  Serial.println(rollOffset);
}

// Check for emergency conditions
bool checkEmergencyConditions() {
  // Check emergency stop button
  if (digitalRead(emergencyStopPin) == LOW) {
    emergencyStopActive = true;
    return true;
  }
  
  // Check for sensor timeout
  if (millis() - lastValidSignal > timeoutMs) {
    Serial.println("WARNING: Sensor timeout detected!");
    return true;
  }
  
  // Check for extended emergency timeout
  if (millis() - lastEmergencyCheck > emergencyTimeoutMs) {
    Serial.println("EMERGENCY: Extended timeout - stopping for safety!");
    return true;
  }
  
  return false;
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Wheelchair Control System Starting...");

  // Initialize emergency stop button
  pinMode(emergencyStopPin, INPUT_PULLUP);

  // Initialize I2C communication
  Wire.begin();

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("CRITICAL ERROR: Failed to find MPU6050 chip");
    sensorError = true;
    emergencyStop();
    while (1) {
      delay(100);
    }
  }

  // Configure MPU6050 settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 Found! Starting calibration...");

  // Set pin modes for motor control
  pinMode(motorKiriIN1, OUTPUT);
  pinMode(motorKiriIN2, OUTPUT);
  pinMode(motorKiriEN, OUTPUT);
  pinMode(motorKananIN1, OUTPUT);
  pinMode(motorKananIN2, OUTPUT);
  pinMode(motorKananEN, OUTPUT);

  // Initial stop
  emergencyStop();
  
  // Calibrate sensor
  calibrateSensor();
  
  Serial.println("System ready! Press emergency stop button to stop.");
  lastValidSignal = millis();
  lastEmergencyCheck = millis();
}

void loop() {
  // Check emergency conditions first
  if (checkEmergencyConditions()) {
    emergencyStop();
    return;
  }
  
  // Reset emergency stop if button is released
  if (digitalRead(emergencyStopPin) == HIGH && emergencyStopActive) {
    emergencyStopActive = false;
    Serial.println("Emergency stop released. System resuming...");
    lastValidSignal = millis();
    lastEmergencyCheck = millis();
  }
  
  // Get sensor events
  sensors_event_t a, g, temp;
  if (!mpu.getEvent(&a, &g, &temp)) {
    Serial.println("ERROR: Failed to read sensor data");
    sensorError = true;
    emergencyStop();
    return;
  }
  
  sensorError = false;

  // Calculate pitch and roll with calibration offset
  float pitch = atan2(a.acceleration.y, a.acceleration.z) * (180.0 / PI) - pitchOffset;
  float roll = atan2(a.acceleration.x, a.acceleration.z) * (180.0 / PI) - rollOffset;

  // Apply low-pass filter (simple moving average)
  static float filteredPitch = 0;
  static float filteredRoll = 0;
  const float filterAlpha = 0.1;
  
  filteredPitch = filterAlpha * pitch + (1 - filterAlpha) * filteredPitch;
  filteredRoll = filterAlpha * roll + (1 - filterAlpha) * filteredRoll;

  // Determine target speed based on tilt magnitude
  int targetSpeed = minSpeed;
  if (abs(filteredPitch) > thresholdMaju || abs(filteredRoll) > thresholdBelok) {
    targetSpeed = maxSpeed;
  }

  // Improved control logic with priority system
  bool commandExecuted = false;
  
  // Priority 1: Forward/Backward (higher priority)
  if (filteredPitch > thresholdMaju) {
    maju(targetSpeed);
    Serial.println("Maju");
    commandExecuted = true;
  } else if (filteredPitch < thresholdMundur) {
    mundur(targetSpeed);
    Serial.println("Mundur");
    commandExecuted = true;
  }
  
  // Priority 2: Left/Right (only if no forward/backward command)
  if (!commandExecuted) {
    if (filteredRoll > thresholdBelok) {
      belokKanan(targetSpeed);
      Serial.println("Belok Kanan");
    } else if (filteredRoll < -thresholdBelok) {
      belokKiri(targetSpeed);
      Serial.println("Belok Kiri");
    } else {
      berhenti();
      Serial.println("Berhenti");
    }
  }

  // Update timestamps
  lastValidSignal = millis();
  lastEmergencyCheck = millis();

  // Debugging output
  Serial.print("Pitch: ");
  Serial.print(filteredPitch);
  Serial.print(" Roll: ");
  Serial.print(filteredRoll);
  Serial.print(" Speed: ");
  Serial.print(currentSpeed);
  Serial.print(" Emergency: ");
  Serial.println(emergencyStopActive ? "YES" : "NO");

  // Safety delay
  delay(50);
}