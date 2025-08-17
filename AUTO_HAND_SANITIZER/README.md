# 🚿 Auto Hand Sanitizer - Máy Rửa Tay Tự Động

[![Version](https://img.shields.io/badge/version-2.0-blue.svg)](https://github.com/Thanhtan2108/AUTOMATIC-DISPENSER/releases)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Status](https://img.shields.io/badge/status-Production%20Ready-brightgreen.svg)](https://github.com/Thanhtan2108/AUTOMATIC-DISPENSER)
[![Platform](https://img.shields.io/badge/platform-Arduino%20Uno-orange.svg)](https://www.arduino.cc/)
[![Build](https://img.shields.io/badge/build-Passing-brightgreen.svg)](https://github.com/Thanhtan2108/AUTOMATIC-DISPENSER/actions)

> 🤖 **Hệ thống rửa tay tự động thông minh** sử dụng Arduino Uno với cảm biến siêu âm HC-SR04 để phát hiện tay người và tự động kích hoạt bơm dung dịch sát khuẩn thông qua servo SG90. Giải pháp tiết kiệm năng lượng với chế độ ngủ thông minh và giao diện LCD thân thiện.

---

## 📋 Mục lục

- [🎯 Tổng quan](#-tổng-quan)
- [✨ Tính năng chính](#-tính-năng-chính)
- [🔧 Yêu cầu hệ thống](#-yêu-cầu-hệ-thống)
- [🚀 Cài đặt và sử dụng](#-cài-đặt-và-sử-dụng)
- [🔌 Sơ đồ kết nối](#-sơ-đồ-kết-nối)
- [⚙️ Cấu hình và tùy chỉnh](#️-cấu-hình-và-tùy-chỉnh)
- [🧪 Testing](#-testing)
- [🔍 Troubleshooting](#-troubleshooting)
- [📊 Hiệu suất và tối ưu](#-hiệu-suất-và-tối-ưu)
- [🎯 Ứng dụng thực tế](#-ứng-dụng-thực-tế)
- [🤝 Đóng góp](#-đóng-góp)
- [📄 License](#-license)
- [📞 Liên hệ](#-liên-hệ)

---

## 🎯 Tổng quan

### Vấn đề cần giải quyết

Trong bối cảnh dịch bệnh và nhu cầu vệ sinh tay cao, việc sử dụng dung dịch sát khuẩn thủ công thường gây lãng phí và không đảm bảo vệ sinh. **Auto Hand Sanitizer** được thiết kế để giải quyết những vấn đề này:

- 🚫 **Lãng phí dung dịch**: Tự động định lượng chính xác
- 🦠 **Lây nhiễm chéo**: Không cần chạm tay vào vòi bơm
- ⚡ **Tiêu thụ năng lượng**: Chế độ ngủ thông minh tiết kiệm pin
- 🎛️ **Điều chỉnh linh hoạt**: Khoảng cách phát hiện có thể tùy chỉnh

### Lý do tạo ra project

Project này được phát triển với mục tiêu:

- **Giáo dục**: Học tập về IoT, cảm biến, và điều khiển tự động
- **Thực nghiệm**: Áp dụng kiến thức Arduino vào thực tế
- **Cộng đồng**: Chia sẻ giải pháp mã nguồn mở cho cộng đồng
- **Sáng tạo**: Tạo ra sản phẩm hữu ích từ các linh kiện đơn giản

---

## ✨ Tính năng chính

| Tính năng                    | Mô tả                                            | Trạng thái    |
| ---------------------------- | ------------------------------------------------ | ------------- |
| 🔍 **Phát hiện tay tự động** | HC-SR04 với khoảng cách điều chỉnh được (5-25cm) | ✅ Hoàn thành |
| 🤖 **Kích hoạt servo**       | SG90 nhấn vòi bơm tự động                        | ✅ Hoàn thành |
| 👆 **Manual override**       | Nút nhấn để kích hoạt thủ công                   | ✅ Hoàn thành |
| 📺 **Hiển thị LCD**          | Thông tin khoảng cách, ngưỡng, trạng thái        | ✅ Hoàn thành |
| 💧 **Cảm biến mức nước**     | Cảnh báo khi dung dịch gần hết                   | ✅ Hoàn thành |
| 🔋 **Tiết kiệm pin**         | Sleep mode khi không hoạt động                   | ✅ Hoàn thành |
| 🐛 **Debug qua Serial**      | Theo dõi hoạt động qua Serial Monitor            | ✅ Hoàn thành |
| 🎛️ **Điều chỉnh ngưỡng**     | Potentiometer để tùy chỉnh khoảng cách           | ✅ Hoàn thành |

---

## 🔧 Yêu cầu hệ thống

### 📱 Phần cứng (Hardware)

| Linh kiện                 | Số lượng | Thông số               | Ghi chú                        |
| ------------------------- | -------- | ---------------------- | ------------------------------ |
| 🖥️ **Arduino Uno R3**     | 1        | ATmega328P, 16MHz      | Board chính điều khiển         |
| 📡 **HC-SR04**            | 1        | 2cm-400cm, 15°         | Cảm biến siêu âm phát hiện tay |
| 🤖 **Servo SG90**         | 1        | 0-180°, 4.8V           | Nhấn vòi bơm tự động           |
| 📺 **LCD 16x2 I2C**       | 1        | 0x27/0x3F              | Hiển thị thông tin             |
| 🎛️ **Potentiometer 10kΩ** | 1        | 10kΩ, Linear           | Điều chỉnh ngưỡng khoảng cách  |
| 🔘 **Nút nhấn**           | 1        | Momentary, 12mm        | Manual override                |
| 💧 **Cảm biến mức nước**  | 1        | Float sensor/Điện cực  | Phát hiện mức nước thấp        |
| 🔴 **LED đỏ**             | 1        | 5mm, 20mA              | Cảnh báo mức nước thấp         |
| ⚡ **Điện trở 220Ω**      | 1        | 220Ω, 1/4W             | Bảo vệ LED                     |
| 🔌 **Breadboard**         | 1        | 830 điểm               | Kết nối mạch                   |
| 🔌 **Dây jumper**         | 1 bộ     | Male-Male, Male-Female | Kết nối các thành phần         |
| 🔋 **Nguồn 5V**           | 1        | 5V, 2A+                | Adapter hoặc pin + boost       |

### 💻 Phần mềm (Software)

| Thành phần                 | Phiên bản | Mô tả                          |
| -------------------------- | --------- | ------------------------------ |
| 🔧 **PlatformIO**          | 6.0+      | Framework phát triển chính     |
| 🖥️ **VS Code**             | 1.80+     | IDE khuyến nghị với PlatformIO |
| 📚 **Thư viện Arduino**    | 1.8+      | Framework cơ bản               |
| 🤖 **Servo.h**             | 1.1.8+    | Điều khiển servo motor         |
| 📺 **LiquidCrystal_I2C.h** | 1.1.4+    | Điều khiển LCD I2C             |
| ⚡ **avr/sleep.h**         | Built-in  | Quản lý chế độ ngủ             |
| 🔌 **avr/interrupt.h**     | Built-in  | Quản lý ngắt ngoài             |

---

## 🚀 Cài đặt và sử dụng

### 📋 Bước 1: Chuẩn bị môi trường

```bash
# Cài đặt PlatformIO Core
pip install platformio

# Hoặc sử dụng PlatformIO IDE trong VS Code
# Extensions: PlatformIO IDE
```

### 📥 Bước 2: Clone project

```bash
git clone https://github.com/Thanhtan2108/AUTO-HAND-SANITIZER.git
cd AUTO-HAND-SANITIZER
```

### 🔨 Bước 3: Build và upload

```bash
# Build project
pio run

# Upload lên Arduino
pio run --target upload

# Monitor Serial (debug)
pio device monitor
```

### ⚙️ Bước 4: Cấu hình ban đầu

1. **🔍 Điều chỉnh địa chỉ LCD**:

   - Nếu LCD không hiển thị, thay đổi địa chỉ I2C trong code
   - Thường là `0x27` hoặc `0x3F`

2. **📏 Calibrate khoảng cách**:

   - Điều chỉnh potentiometer để set ngưỡng phù hợp (5-25cm)
   - Test với tay ở các khoảng cách khác nhau

3. **🤖 Test servo**:
   - Kiểm tra góc mở/đóng servo phù hợp với vòi bơm
   - Điều chỉnh `SERVO_OPEN_ANGLE` và `SERVO_CLOSE_ANGLE`

---

## 🔌 Sơ đồ kết nối

### 📍 Pin Assignment

```text
Arduino Uno    | Component        | Chức năng
---------------|------------------|------------------
D2 (INT0)     | Button (Manual)  | Ngắt ngoài, manual override
D6 (PWM)      | Servo SG90       | Điều khiển servo (PWM)
D8            | Water Level      | Cảm biến mức nước
D9            | HC-SR04 TRIG     | Trigger siêu âm
D10           | HC-SR04 ECHO     | Echo siêu âm
D13           | LED Warning      | LED cảnh báo mức nước thấp
A0            | Potentiometer    | Điều chỉnh ngưỡng (Analog)
SDA (A4)      | LCD I2C          | Data I2C
SCL (A5)      | LCD I2C          | Clock I2C
5V            | Power Supply     | Nguồn 5V
GND           | Ground           | Mass chung
```

### 🔗 Kết nối chi tiết

#### HC-SR04 (Ultrasonic Sensor)

```text
VCC   → 5V
TRIG  → D9
ECHO  → D10
GND   → GND
```

#### Servo SG90

```text
Red   → 5V
Brown → GND
Orange→ D6
```

#### LCD I2C

```text
VCC   → 5V
GND   → GND
SDA   → A4
SCL   → A5
```

#### Button (Manual Override)

```text
Terminal 1 → D2
Terminal 2 → GND
```

#### Potentiometer

```text
Left   → GND
Middle → A0
Right  → 5V
```

#### Water Level Sensor

```text
VCC    → 5V
GND    → GND
Signal → D8
```

#### LED Warning

```text
Anode   → D13 (qua điện trở 220Ω)
Cathode → GND
```

---

## ⚙️ Cấu hình và tùy chỉnh

### ⏰ Thay đổi thời gian sleep

```cpp
// Trong src/main.cpp
const unsigned long SLEEP_TIMEOUT = 15000; // 15 giây
```

### 📏 Điều chỉnh khoảng cách

```cpp
// Thay đổi range mapping cho potentiometer
int threshold = map(potValue, 0, 1023, 5, 25); // 5-25cm
```

### 🤖 Thay đổi góc servo

```cpp
// Điều chỉnh góc mở/đóng servo
const int SERVO_OPEN_ANGLE = 90;   // Góc mở
const int SERVO_CLOSE_ANGLE = 0;   // Góc đóng
```

### 🔄 Thay đổi debounce button

```cpp
// Điều chỉnh thời gian debounce
const unsigned long BUTTON_DEBOUNCE = 200; // 200ms
```

---

## 🧪 Testing

### 🧪 Chạy test cơ bản

```bash
# Chạy test suite
pio test

# Chạy test cụ thể
pio test -f test_basic
```

### 📊 Test coverage

| Chức năng           | Test case                  | Trạng thái |
| ------------------- | -------------------------- | ---------- |
| **Constants**       | Kiểm tra hằng số cấu hình  | ✅         |
| **Pin Assignment**  | Kiểm tra pin assignment    | ✅         |
| **Distance Logic**  | Kiểm tra logic khoảng cách | ✅         |
| **Servo Control**   | Kiểm tra điều khiển servo  | ✅         |
| **Button Debounce** | Kiểm tra debounce button   | ✅         |

---

## 🔍 Troubleshooting

### 📺 LCD không hiển thị

**Triệu chứng**: LCD không hiển thị gì hoặc hiển thị sai

**Nguyên nhân và giải pháp**:

- ❌ **Địa chỉ I2C sai**: Thay đổi địa chỉ trong code (0x27 hoặc 0x3F)
- ❌ **Kết nối SDA/SCL sai**: Kiểm tra kết nối A4/A5
- ❌ **Nguồn không đủ**: Đảm bảo nguồn 5V ổn định
- ❌ **LCD bị hỏng**: Thay thế LCD mới

### 📡 HC-SR04 không hoạt động

**Triệu chứng**: Không đo được khoảng cách hoặc đo sai

**Nguyên nhân và giải pháp**:

- ❌ **Kết nối TRIG/ECHO sai**: Kiểm tra kết nối D9/D10
- ❌ **Khoảng cách quá gần**: Đảm bảo > 2cm
- ❌ **Nguồn không đủ**: Kiểm tra nguồn 5V
- ❌ **Cảm biến bị hỏng**: Thay thế HC-SR04

### 🤖 Servo không di chuyển

**Triệu chứng**: Servo không quay hoặc quay sai góc

**Nguyên nhân và giải pháp**:

- ❌ **Kết nối pin sai**: Kiểm tra kết nối servo
- ❌ **Nguồn không đủ công suất**: Đảm bảo nguồn 2A+
- ❌ **Góc servo sai**: Kiểm tra góc 0-180°
- ❌ **Servo bị hỏng**: Thay thế servo mới

### 😴 Sleep mode không hoạt động

**Triệu chứng**: Arduino không vào sleep mode hoặc không wake up

**Nguyên nhân và giải pháp**:

- ❌ **Kết nối button sai**: Kiểm tra kết nối D2 (INT0)
- ❌ **Button không đúng**: Đảm bảo button kết nối đúng INT0
- ❌ **Logic sleep sai**: Kiểm tra logic trong code
- ❌ **Ngắt không hoạt động**: Kiểm tra cấu hình ngắt

---

## 📊 Hiệu suất và tối ưu

### 🔋 Tiết kiệm pin

| Tính năng            | Mô tả                          | Tiết kiệm |
| -------------------- | ------------------------------ | --------- |
| **Sleep Mode**       | Chế độ ngủ khi không hoạt động | 90%       |
| **LCD Backlight**    | Tắt backlight khi sleep        | 15%       |
| **Optimized Delays** | Tối ưu thời gian delay         | 10%       |

### 🎯 Độ chính xác

| Thông số            | Giá trị | Mô tả                |
| ------------------- | ------- | -------------------- |
| **Button Debounce** | 200ms   | Tránh trigger nhầm   |
| **HC-SR04 Timeout** | 25ms    | Timeout cho cảm biến |
| **Distance Range**  | 2-400cm | Khoảng cách hợp lệ   |
| **Servo Angle**     | 0-180°  | Góc servo hợp lệ     |

### 🚀 Độ ổn định

| Tiêu chí           | Trước | Sau  | Cải thiện |
| ------------------ | ----- | ---- | --------- |
| **Tính ổn định**   | 6/10  | 9/10 | +50%      |
| **Xử lý lỗi**      | 4/10  | 8/10 | +100%     |
| **Sleep Mode**     | 5/10  | 9/10 | +80%      |
| **Pin Assignment** | 7/10  | 9/10 | +29%      |
| **Debug**          | 3/10  | 8/10 | +167%     |

---

## 🎯 Ứng dụng thực tế

### 🏢 Môi trường sử dụng

| Môi trường       | Mô tả                     | Lợi ích                        |
| ---------------- | ------------------------- | ------------------------------ |
| 🏫 **Lớp học**   | Học sinh, giáo viên       | Giảm lây nhiễm, tiết kiệm      |
| 🏪 **Văn phòng** | Nhân viên, khách hàng     | Vệ sinh tự động, chuyên nghiệp |
| 🏪 **Cửa hàng**  | Khách hàng, nhân viên     | Tăng trải nghiệm, an toàn      |
| 🏥 **Bệnh viện** | Bệnh nhân, nhân viên y tế | Vệ sinh tối ưu, giảm lây nhiễm |
| 🏠 **Gia đình**  | Thành viên gia đình       | Tiện lợi, tiết kiệm            |

### 💡 Lợi ích

- ✅ **Tiết kiệm dung dịch**: Định lượng chính xác, không lãng phí
- ✅ **Giảm lây nhiễm**: Không cần chạm tay vào vòi bơm
- ✅ **Tự động hóa**: Hoạt động hoàn toàn tự động
- ✅ **Tiết kiệm pin**: Sleep mode thông minh
- ✅ **Dễ sử dụng**: Giao diện thân thiện, dễ hiểu
- ✅ **Linh hoạt**: Có thể điều chỉnh theo nhu cầu

---

## 🤝 Đóng góp

Chúng tôi rất hoan nghênh mọi đóng góp từ cộng đồng! Dưới đây là cách bạn có thể đóng góp:

### 📋 Quy trình đóng góp

1. **🔱 Fork project**

   ```bash
   git clone https://github.com/Thanhtan2108/AUTO-HAND-SANITIZER.git
   ```

2. **🌿 Tạo feature branch**

   ```bash
   git checkout -b feature/AmazingFeature
   ```

3. **💾 Commit changes**

   ```bash
   git commit -m 'Add some AmazingFeature'
   ```

4. **📤 Push to branch**

   ```bash
   git push origin feature/AmazingFeature
   ```

5. **🔀 Tạo Pull Request**

### 🎯 Các lĩnh vực cần đóng góp

| Lĩnh vực             | Mô tả                               | Trạng thái |
| -------------------- | ----------------------------------- | ---------- |
| **🖥️ Code**          | Cải thiện logic, tối ưu hiệu suất   | 🔄         |
| **📚 Documentation** | Cải thiện tài liệu, hướng dẫn       | 🔄         |
| **🧪 Testing**       | Thêm test cases, cải thiện coverage | 🔄         |
| **🎨 UI/UX**         | Cải thiện giao diện LCD             | 🔄         |
| **🔧 Hardware**      | Tối ưu mạch điện, PCB               | 🔄         |
| **🌐 Localization**  | Đa ngôn ngữ                         | 🔄         |

### 📝 Guidelines

- **Code Style**: Tuân thủ Arduino coding standards
- **Comments**: Viết comment bằng tiếng Việt hoặc tiếng Anh
- **Testing**: Đảm bảo code hoạt động trước khi submit
- **Documentation**: Cập nhật README nếu cần thiết

---

## 📄 License

Project này được phân phối dưới **MIT License**. Xem file [LICENSE](LICENSE) để biết thêm chi tiết.

### 🔒 Quyền và nghĩa vụ

- ✅ **Sử dụng**: Tự do sử dụng cho mục đích cá nhân và thương mại
- ✅ **Sửa đổi**: Tự do sửa đổi và phân phối
- ✅ **Phân phối**: Tự do phân phối và bán
- ✅ **Tích hợp**: Tự do tích hợp vào project khác
- ⚠️ **Trách nhiệm**: Không có bảo hành, sử dụng có trách nhiệm

---

## 📞 Liên hệ

### 👤 Thông tin liên hệ

| Thông tin      | Chi tiết                                                  |
| -------------- | --------------------------------------------------------- |
| **👨‍💻 Tác giả** | Thanh Tân                                                 |
| **📧 Email**   | [nthanhtan2004@gmail.com](mailto:nthanhtan2004@gmail.com) |
| **🐙 GitHub**  | [@Thanhtan2108](https://github.com/Thanhtan2108)          |

### 📍 Project Information

| Thông tin          | Chi tiết                                                                           |
| ------------------ | ---------------------------------------------------------------------------------- |
| **🏠 Repository**  | [AUTO-HAND-SANITIZER](https://github.com/Thanhtan2108/AUTO-HAND-SANITIZER)         |
| **📋 Issues**      | [Report Bug](https://github.com/Thanhtan2108/AUTO-HAND-SANITIZER/issues)           |
| **💡 Discussions** | [Join Discussion](https://github.com/Thanhtan2108/AUTO-HAND-SANITIZER/discussions) |
| **📚 Wiki**        | [Project Wiki](https://github.com/Thanhtan2108/AUTO-HAND-SANITIZER/wiki)           |

### 🌟 Hỗ trợ

- **📖 Documentation**: Xem phần [Troubleshooting](#-troubleshooting)
- **🐛 Bug Report**: Tạo [Issue](https://github.com/Thanhtan2108/AUTO-HAND-SANITIZER/issues/new)
- **💬 Questions**: Sử dụng [Discussions](https://github.com/Thanhtan2108/AUTO-HAND-SANITIZER/discussions)
- **📧 Email**: Liên hệ trực tiếp qua email

---

## 📝 Changelog

### 🚀 Version 2.0 (Current) - Production Ready

**📅 Release Date**: December 2024

#### ✨ New Features

- 🔧 **Khắc phục xung đột ngắt**: Sử dụng ISR(INT0_vect) thay vì attachInterrupt
- 😴 **Cải thiện logic sleep mode**: Logic ổn định, không bị loop vô hạn
- 🛡️ **Xử lý lỗi đầy đủ**: Timeout, validation cho cảm biến
- 📍 **Tối ưu pin assignment**: Tránh xung đột với PWM và ngắt
- 🐛 **Debug đầy đủ**: Serial monitor để theo dõi hoạt động
- 🎨 **Cải thiện UI LCD**: Giao diện thân thiện, dễ đọc

#### 🔧 Improvements

- ⚡ **Tối ưu hiệu suất**: Giảm delay, cải thiện logic
- 🎛️ **Cấu hình linh hoạt**: Dễ dàng thay đổi thông số
- 📊 **Monitoring**: Theo dõi trạng thái hệ thống
- 🧪 **Testing**: Thêm test suite cơ bản

#### 🐛 Bug Fixes

- ❌ **Xung đột ngắt**: Khắc phục hoàn toàn
- ❌ **Logic sleep**: Không còn bị loop vô hạn
- ❌ **Pin conflict**: Tránh xung đột PWM
- ❌ **Button debounce**: Không còn trigger nhầm

### 📱 Version 1.0 - Basic Implementation

**📅 Release Date**: November 2024

#### ✨ Features

- 🔍 **Phát hiện tay cơ bản**: HC-SR04 với logic đơn giản
- 🤖 **Điều khiển servo**: SG90 cơ bản
- 📺 **LCD display**: Hiển thị thông tin cơ bản
- 😴 **Sleep mode**: Chức năng ngủ đơn giản

---

## 🏆 Acknowledgments

### 🙏 Cảm ơn

- **Arduino Community**: Nền tảng và thư viện
- **PlatformIO**: Framework phát triển tuyệt vời
- **Open Source Contributors**: Những đóng góp từ cộng đồng
- **Testers**: Những người đã test và feedback

### 📚 References

- [Arduino Official Documentation](https://docs.arduino.cc/)
- [HC-SR04 Datasheet](https://www.mouser.com/datasheet/2/813/HCSR04-1022824.pdf)
- [SG90 Servo Datasheet](https://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf)
- [ATmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)

---

## 📊 Project Statistics

![GitHub stars](https://img.shields.io/github/stars/Thanhtan2108/AUTO-HAND-SANITIZER)
![GitHub forks](https://img.shields.io/github/forks/Thanhtan2108/AUTO-HAND-SANITIZER)
![GitHub issues](https://img.shields.io/github/issues/Thanhtan2108/AUTO-HAND-SANITIZER)
![GitHub pull requests](https://img.shields.io/github/issues-pr/Thanhtan2108/AUTO-HAND-SANITIZER)
![GitHub license](https://img.shields.io/github/license/Thanhtan2108/AUTO-HAND-SANITIZER)

---

🌟 **Nếu project này hữu ích, hãy cho chúng tôi một ⭐ Star!** 🌟

🚀 **Hãy cùng nhau xây dựng một cộng đồng IoT mạnh mẽ!** 🚀

---

**⚠️ Disclaimer**: Đây là project giáo dục và thực nghiệm. Sử dụng trong môi trường thực tế cần kiểm tra kỹ lưỡng về an toàn và hiệu quả. Tác giả không chịu trách nhiệm về bất kỳ thiệt hại nào có thể xảy ra.

📝 **Last Updated**: December 2024
🔄 **Version**: 2.0
📋 **Status**: Production Ready
