#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// === Khai báo đối tượng ===
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Địa chỉ 0x27 thường gặp, nếu không nhận thì thử 0x3F

// === Pin - Tối ưu để tránh xung đột ===
const int trigPin = 9;      // D9 - không xung đột với PWM
const int echoPin = 10;     // D10 - không xung đột với PWM  
const int servoPin = 6;     // D6 - PWM pin cho servo
const int buttonPin = 2;    // D2 = INT0 - ngắt ngoài
const int potPin = A0;      // A0 - analog input
const int waterSensorPin = 8; // D8 - digital input
const int ledPin = 13;      // D13 - built-in LED

// === Biến trạng thái ===
volatile bool wakeUpFlag = false;
volatile bool buttonPressed = false;
unsigned long lastButtonPress = 0;
unsigned long lastActiveTime = 0;
bool isSleeping = false;

// === Hằng số cấu hình ===
const unsigned long SLEEP_TIMEOUT = 15000;    // 15 giây không hoạt động thì sleep
const unsigned long BUTTON_DEBOUNCE = 200;    // 200ms debounce
const int SERVO_OPEN_ANGLE = 90;
const int SERVO_CLOSE_ANGLE = 0;
const int MIN_DISTANCE = 2;                   // Khoảng cách tối thiểu hợp lệ (cm)
const int MAX_DISTANCE = 400;                 // Khoảng cách tối đa hợp lệ (cm)

// === ISR cho INT0 (button) - Cách duy nhất, không dùng attachInterrupt ===
ISR(INT0_vect) {
  if (millis() - lastButtonPress > BUTTON_DEBOUNCE) {
    wakeUpFlag = true;
    buttonPressed = true;
    lastButtonPress = millis();
  }
}

// === Cấu hình ngắt INT0 ===
void setupInterrupt() {
  cli(); // Tắt ngắt tạm thời
  
  // Cấu hình INT0 theo datasheet ATmega328P
  EICRA &= ~(1 << ISC01);  // Clear bit ISC01
  EICRA &= ~(1 << ISC00);  // Clear bit ISC00
  EICRA |= (1 << ISC01);   // Set ISC01 = 1, ISC00 = 0 -> Falling edge
  
  EIMSK |= (1 << INT0);    // Enable INT0
  
  sei(); // Bật lại ngắt
}

// === Sleep mode cải tiến ===
void goToSleep() {
  if (isSleeping) return; // Tránh sleep nhiều lần
  
  isSleeping = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sleeping...");
  delay(500);

  // Tắt các thiết bị không cần thiết
  lcd.noBacklight();
  digitalWrite(ledPin, LOW);
  
  // Cấu hình sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  
  // Xóa cờ ngắt cũ và bật ngắt
  cli();
  EIFR = bit(INTF0); // Clear INT0 flag
  sei();
  
  sleep_cpu(); // Zzz...
  
  // Khi có ngắt INT0, CPU tỉnh dậy
  sleep_disable();
  isSleeping = false;
  
  // Khôi phục thiết bị
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Woke up!");
  delay(500);
  
  // Reset timer
  lastActiveTime = millis();
}

// === Đọc khoảng cách với xử lý lỗi ===
int readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Đọc echo với timeout 25ms (khoảng 4m)
  unsigned long duration = pulseIn(echoPin, HIGH, 25000UL);
  
  if (duration == 0) {
    return -1; // Timeout - không có vật thể trong tầm
  }
  
  int distance = duration * 0.0343 / 2;
  
  // Kiểm tra khoảng cách hợp lệ
  if (distance < MIN_DISTANCE || distance > MAX_DISTANCE) {
    return -1;
  }
  
  return distance;
}

// === Xử lý servo với bảo vệ ===
void controlServo(int angle) {
  if (angle < 0 || angle > 180) return; // Bảo vệ góc servo
  
  myServo.write(angle);
  delay(500); // Đợi servo di chuyển
}

// === Xử lý button với debounce ===
bool isButtonPressed() {
  if (buttonPressed) {
    buttonPressed = false;
    return true;
  }
  return false;
}

void setup() {
  // Khởi tạo Serial để debug
  Serial.begin(9600);
  Serial.println("Auto Hand Sanitizer Starting...");
  
  // Cấu hình pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(waterSensorPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  
  // Khởi tạo servo
  myServo.attach(servoPin);
  controlServo(SERVO_CLOSE_ANGLE);
  
  // Khởi tạo LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Auto Dispenser");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1000);
  
  // Cấu hình ngắt
  setupInterrupt();
  
  // Khởi tạo timer
  lastActiveTime = millis();
  
  Serial.println("Setup completed!");
}

void loop() {
  // === Đọc cảm biến ===
  int distance = readDistance();
  int potValue = analogRead(potPin);
  int threshold = map(potValue, 0, 1023, 5, 25); // 5-25cm
  bool waterLow = (digitalRead(waterSensorPin) == LOW);
  
  // === Xử lý button manual ===
  if (isButtonPressed()) {
    Serial.println("Manual button pressed!");
    if (!waterLow) {
      lcd.setCursor(10, 0);
      lcd.print("MANUAL");
      controlServo(SERVO_OPEN_ANGLE);
      delay(2000);
      controlServo(SERVO_CLOSE_ANGLE);
      lastActiveTime = millis(); // Reset timer
    }
  }
  
  // === Hiển thị LCD ===
  lcd.clear();
  lcd.setCursor(0, 0);
  if (distance > 0) {
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print("cm ");
  } else {
    lcd.print("No Object    ");
  }
  
  lcd.setCursor(0, 1);
  lcd.print("Thr: ");
  lcd.print(threshold);
  lcd.print("cm ");
  
  // === Kiểm tra mức nước ===
  if (waterLow) {
    lcd.setCursor(10, 1);
    lcd.print("LOW!");
    digitalWrite(ledPin, HIGH);
  } else {
    lcd.setCursor(10, 1);
    lcd.print("OK  ");
    digitalWrite(ledPin, LOW);
  }
  
  // === Logic chính - Phát hiện tay và kích hoạt ===
  if (!waterLow && distance > 0 && distance < threshold) {
    lcd.setCursor(10, 0);
    lcd.print("DISPENSE");
    Serial.print("Hand detected at ");
    Serial.print(distance);
    Serial.println("cm");
    
    controlServo(SERVO_OPEN_ANGLE);
    delay(2000);
    controlServo(SERVO_CLOSE_ANGLE);
    
    lastActiveTime = millis(); // Reset timer
  } else if (distance > 0) {
    lcd.setCursor(10, 0);
    lcd.print("READY ");
  }
  
  // === Logic sleep mode cải tiến ===
  if (distance > threshold || distance == -1) {
    // Không có tay trong tầm
    if (millis() - lastActiveTime > SLEEP_TIMEOUT && !isSleeping) {
      Serial.println("Going to sleep...");
      goToSleep();
    }
  } else {
    // Có tay trong tầm hoặc vừa hoạt động
    lastActiveTime = millis();
  }
  
  // Debug info qua Serial
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print("cm, Threshold: ");
  Serial.print(threshold);
  Serial.print("cm, Water: ");
  Serial.print(waterLow ? "LOW" : "OK");
  Serial.print(", Time to sleep: ");
  Serial.println((SLEEP_TIMEOUT - (millis() - lastActiveTime)) / 1000);
  
  delay(200); // Delay chính
}
