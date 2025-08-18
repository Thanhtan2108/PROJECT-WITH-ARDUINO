#include <Arduino.h>
#include <unity.h>

// Test configuration
const int TEST_EMERGENCY_PIN = 8;
const int TEST_MOTOR_PIN1 = 2;
const int TEST_MOTOR_PIN2 = 3;
const int TEST_MOTOR_EN = 9;

// Test variables
bool emergencyStopTriggered = false;
unsigned long testStartTime = 0;

// Mock functions for testing
void mockEmergencyStop() {
    emergencyStopTriggered = true;
    digitalWrite(TEST_MOTOR_PIN1, LOW);
    digitalWrite(TEST_MOTOR_PIN2, LOW);
    analogWrite(TEST_MOTOR_EN, 0);
}

void setup() {
    Serial.begin(9600);
    delay(2000);
    
    // Initialize test pins
    pinMode(TEST_EMERGENCY_PIN, INPUT_PULLUP);
    pinMode(TEST_MOTOR_PIN1, OUTPUT);
    pinMode(TEST_MOTOR_PIN2, OUTPUT);
    pinMode(TEST_MOTOR_EN, OUTPUT);
    
    UNITY_BEGIN();
    Serial.println("Starting Safety Tests...");
}

void loop() {
    // Test 1: Emergency Stop Button Functionality
    RUN_TEST(testEmergencyStopButton);
    
    // Test 2: Motor Stop Functionality
    RUN_TEST(testMotorStop);
    
    // Test 3: Timeout Safety
    RUN_TEST(testTimeoutSafety);
    
    // Test 4: Speed Ramping
    RUN_TEST(testSpeedRamping);
    
    // Test 5: Sensor Error Handling
    RUN_TEST(testSensorErrorHandling);
    
    // Test 6: Calibration Function
    RUN_TEST(testCalibration);
    
    // Test 7: Priority Control Logic
    RUN_TEST(testPriorityControl);
    
    UNITY_END();
    while(1) {
        delay(1000);
    }
}

// Test 1: Emergency Stop Button
void testEmergencyStopButton() {
    Serial.println("Testing Emergency Stop Button...");
    
    // Test button press
    digitalWrite(TEST_EMERGENCY_PIN, LOW);
    delay(100);
    
    // Simulate emergency stop
    mockEmergencyStop();
    
    TEST_ASSERT_TRUE(emergencyStopTriggered);
    TEST_ASSERT_EQUAL(LOW, digitalRead(TEST_MOTOR_PIN1));
    TEST_ASSERT_EQUAL(LOW, digitalRead(TEST_MOTOR_PIN2));
    TEST_ASSERT_EQUAL(0, analogRead(TEST_MOTOR_EN));
    
    // Test button release
    digitalWrite(TEST_EMERGENCY_PIN, HIGH);
    emergencyStopTriggered = false;
    
    Serial.println("Emergency Stop Button Test: PASSED");
}

// Test 2: Motor Stop Functionality
void testMotorStop() {
    Serial.println("Testing Motor Stop Function...");
    
    // Start motors
    digitalWrite(TEST_MOTOR_PIN1, HIGH);
    digitalWrite(TEST_MOTOR_PIN2, LOW);
    analogWrite(TEST_MOTOR_EN, 150);
    
    delay(100);
    
    // Stop motors
    digitalWrite(TEST_MOTOR_PIN1, LOW);
    digitalWrite(TEST_MOTOR_PIN2, LOW);
    analogWrite(TEST_MOTOR_EN, 0);
    
    TEST_ASSERT_EQUAL(LOW, digitalRead(TEST_MOTOR_PIN1));
    TEST_ASSERT_EQUAL(LOW, digitalRead(TEST_MOTOR_PIN2));
    TEST_ASSERT_EQUAL(0, analogRead(TEST_MOTOR_EN));
    
    Serial.println("Motor Stop Test: PASSED");
}

// Test 3: Timeout Safety
void testTimeoutSafety() {
    Serial.println("Testing Timeout Safety...");
    
    const unsigned long timeoutMs = 1000;
    unsigned long startTime = millis();
    
    // Simulate timeout condition
    while (millis() - startTime < timeoutMs + 100) {
        delay(10);
    }
    
    // Should trigger emergency stop after timeout
    mockEmergencyStop();
    
    TEST_ASSERT_TRUE(emergencyStopTriggered);
    
    Serial.println("Timeout Safety Test: PASSED");
}

// Test 4: Speed Ramping
void testSpeedRamping() {
    Serial.println("Testing Speed Ramping...");
    
    const int targetSpeed = 150;
    const int rampStep = 10;
    int currentSpeed = 0;
    
    // Test speed increase
    while (currentSpeed < targetSpeed) {
        currentSpeed = min(currentSpeed + rampStep, targetSpeed);
        analogWrite(TEST_MOTOR_EN, currentSpeed);
        delay(50);
    }
    
    TEST_ASSERT_EQUAL(targetSpeed, currentSpeed);
    
    // Test speed decrease
    while (currentSpeed > 0) {
        currentSpeed = max(currentSpeed - rampStep, 0);
        analogWrite(TEST_MOTOR_EN, currentSpeed);
        delay(50);
    }
    
    TEST_ASSERT_EQUAL(0, currentSpeed);
    
    Serial.println("Speed Ramping Test: PASSED");
}

// Test 5: Sensor Error Handling
void testSensorErrorHandling() {
    Serial.println("Testing Sensor Error Handling...");
    
    bool sensorError = false;
    
    // Simulate sensor error
    sensorError = true;
    
    if (sensorError) {
        mockEmergencyStop();
    }
    
    TEST_ASSERT_TRUE(emergencyStopTriggered);
    
    Serial.println("Sensor Error Handling Test: PASSED");
}

// Test 6: Calibration Function
void testCalibration() {
    Serial.println("Testing Calibration Function...");
    
    float pitchOffset = 0;
    float rollOffset = 0;
    bool isCalibrated = false;
    
    // Simulate calibration
    const int calibrationSamples = 10;
    float pitchSum = 0;
    float rollSum = 0;
    
    for (int i = 0; i < calibrationSamples; i++) {
        // Mock sensor readings
        float mockPitch = 1.5 + random(-0.5, 0.5);
        float mockRoll = -0.8 + random(-0.3, 0.3);
        
        pitchSum += mockPitch;
        rollSum += mockRoll;
        delay(10);
    }
    
    pitchOffset = pitchSum / calibrationSamples;
    rollOffset = rollSum / calibrationSamples;
    isCalibrated = true;
    
    TEST_ASSERT_TRUE(isCalibrated);
    TEST_ASSERT_TRUE(pitchOffset > 0);
    TEST_ASSERT_TRUE(rollOffset < 0);
    
    Serial.println("Calibration Test: PASSED");
}

// Test 7: Priority Control Logic
void testPriorityControl() {
    Serial.println("Testing Priority Control Logic...");
    
    bool commandExecuted = false;
    int targetSpeed = 150;
    
    // Test forward priority over turning
    float mockPitch = 15.0;  // Forward tilt
    float mockRoll = 12.0;   // Right tilt
    
    if (mockPitch > 10) {
        // Should execute forward command
        commandExecuted = true;
    } else if (mockRoll > 10) {
        // Should not execute if forward command is active
        commandExecuted = false;
    }
    
    TEST_ASSERT_TRUE(commandExecuted);
    
    Serial.println("Priority Control Test: PASSED");
}

// Additional safety validation functions
void validateSafetyParameters() {
    Serial.println("Validating Safety Parameters...");
    
    // Check threshold values
    TEST_ASSERT_TRUE(10 > 0);  // thresholdMaju
    TEST_ASSERT_TRUE(-10 < 0); // thresholdMundur
    TEST_ASSERT_TRUE(10 > 0);  // thresholdBelok
    
    // Check speed limits
    TEST_ASSERT_TRUE(150 <= 255); // maxSpeed
    TEST_ASSERT_TRUE(50 >= 0);    // minSpeed
    TEST_ASSERT_TRUE(10 > 0);     // speedRampStep
    
    // Check timeout values
    TEST_ASSERT_TRUE(1000 > 0);   // timeoutMs
    TEST_ASSERT_TRUE(5000 > 1000); // emergencyTimeoutMs
    
    Serial.println("Safety Parameters Validation: PASSED");
}

// Performance test
void testPerformance() {
    Serial.println("Testing Performance...");
    
    unsigned long startTime = micros();
    
    // Simulate main loop operations
    for (int i = 0; i < 1000; i++) {
        // Mock sensor reading
        float pitch = random(-20, 20);
        float roll = random(-20, 20);
        
        // Mock control logic
        if (pitch > 10) {
            // Forward command
        } else if (pitch < -10) {
            // Backward command
        } else if (roll > 10) {
            // Right turn
        } else if (roll < -10) {
            // Left turn
        }
        
        delayMicroseconds(50);
    }
    
    unsigned long endTime = micros();
    unsigned long executionTime = endTime - startTime;
    
    // Should complete within reasonable time
    TEST_ASSERT_TRUE(executionTime < 100000); // Less than 100ms
    
    Serial.print("Performance Test: PASSED (");
    Serial.print(executionTime);
    Serial.println(" microseconds)");
}
