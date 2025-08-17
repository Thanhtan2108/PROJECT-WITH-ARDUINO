#include <unity.h>
#include <Arduino.h>

// Test các hằng số cấu hình
void test_constants() {
    TEST_ASSERT_EQUAL(15000, SLEEP_TIMEOUT);
    TEST_ASSERT_EQUAL(200, BUTTON_DEBOUNCE);
    TEST_ASSERT_EQUAL(90, SERVO_OPEN_ANGLE);
    TEST_ASSERT_EQUAL(0, SERVO_CLOSE_ANGLE);
    TEST_ASSERT_EQUAL(2, MIN_DISTANCE);
    TEST_ASSERT_EQUAL(400, MAX_DISTANCE);
}

// Test pin assignment
void test_pin_assignment() {
    TEST_ASSERT_EQUAL(9, trigPin);
    TEST_ASSERT_EQUAL(10, echoPin);
    TEST_ASSERT_EQUAL(6, servoPin);
    TEST_ASSERT_EQUAL(2, buttonPin);
    TEST_ASSERT_EQUAL(A0, potPin);
    TEST_ASSERT_EQUAL(8, waterSensorPin);
    TEST_ASSERT_EQUAL(13, ledPin);
}

// Test logic khoảng cách
void test_distance_logic() {
    // Test khoảng cách hợp lệ
    TEST_ASSERT_TRUE(readDistance() >= 0 || readDistance() == -1);
    
    // Test ngưỡng mapping
    int threshold = map(0, 0, 1023, 5, 25);
    TEST_ASSERT_EQUAL(5, threshold);
    
    threshold = map(1023, 0, 1023, 5, 25);
    TEST_ASSERT_EQUAL(25, threshold);
}

// Test servo control
void test_servo_control() {
    // Test góc hợp lệ
    controlServo(0);
    controlServo(90);
    controlServo(180);
    
    // Test góc không hợp lệ
    controlServo(-1);  // Không nên thực hiện
    controlServo(181); // Không nên thực hiện
}

// Test button debounce
void test_button_debounce() {
    // Simulate button press
    buttonPressed = true;
    lastButtonPress = millis();
    
    TEST_ASSERT_TRUE(isButtonPressed());
    TEST_ASSERT_FALSE(buttonPressed); // Should be reset
    
    // Test debounce timing
    buttonPressed = true;
    lastButtonPress = millis() - 100; // Less than debounce time
    
    // Should not trigger due to debounce
    // This test would need more sophisticated timing simulation
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    
    RUN_TEST(test_constants);
    RUN_TEST(test_pin_assignment);
    RUN_TEST(test_distance_logic);
    RUN_TEST(test_servo_control);
    RUN_TEST(test_button_debounce);
    
    UNITY_END();
}

void loop() {
    // Test framework handles the loop
}
