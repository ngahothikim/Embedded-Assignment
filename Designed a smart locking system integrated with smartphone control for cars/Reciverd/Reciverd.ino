// Thư viện cần thiết
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h >
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

// Định nghĩa các chân kết nối
#define CE_PIN              4   // Chân CE của nRF24L01
#define CSN_PIN             5   // Chân CSN (CS) của nRF24L01

#define MPU_POWER_PIN       13  // Chân điều khiển nguồn cho MPU6050
#define DHT_PIN             16  // Chân DATA của DHT11
#define CB_VOL_ACCU         35  // Chân đọc điện áp ắc quy (ADC1_CH7)
#define CB_P_RON            15  // Chân đọc cảm biến P-Ron (ADC1_CH4)
#define CB_ENGINE           32  // Chân đọc trạng thái động cơ
#define CB_KEY_POWER        33  // Chân đọc trạng thái khóa (ADC1_CH5)

#define RELAY_POWER_KEY_PIN 25  // Chân điều khiển relay nguồn khóa
#define RELAY_DAYLIGHT_PIN  26  // Chân điều khiển relay đèn ban ngày
#define RELAY_BEEPER_PIN    27  // Chân điều khiển relay còi
#define RELAY_HEADLIGHT_PIN 12  // Chân điều khiển relay đèn pha
#define RELAY_DC_OUT_PIN    14  // Chân điều khiển relay DC out

// Khởi tạo các đối tượng cho các thiết bị
RF24 radio(CE_PIN, CSN_PIN);                           // Đối tượng RF24 cho nRF24L01
Adafruit_SSD1306 display(128, 32, &Wire);              // Đối tượng OLED SSD1306
Adafruit_MPU6050 mpu;                                  // Đối tượng MPU6050
DHT dht(DHT_PIN, DHT11);                               // Đối tượng DHT11

// Địa chỉ cho nRF24L01
const byte addresses[][6] = {"5729", "2989"};

// Biến toàn cục cho chương trình
boolean power = 0, headlight = 0, dcout = 0, warning = 0, checkwarning = 0;
int alarmstate = 0, accu = 0, lockStatus = 0, tem = 0, hum = 0, setting = 0, autoReset = 0;
float battery = 0.0;
const float VbatMax = 15;
char confirm[50] = ""; // Tăng kích thước nếu cần
String datasend = "";
unsigned long timeSave = 0, checkEngine = 0, timeRefesh = 0;
unsigned long lastPrintTime = 0;
float gyroBiasX = 0.0;
float gyroBiasY = 0.0;
float gyroBiasZ = 0.0;
const int calibrationSamples = 100; // Số lượng mẫu để hiệu chỉnh

void setup() {
  Serial.begin(115200); // Tăng baud rate để phù hợp với ESP32

  // Khởi tạo các chân GPIO
  pinMode(MPU_POWER_PIN, OUTPUT);        // Chân điều khiển nguồn cho MPU6050
  pinMode(CB_VOL_ACCU, INPUT);           // Chân đọc điện áp ắc quy
  pinMode(CB_P_RON, INPUT);              // Chân đọc cảm biến P-Ron
  pinMode(CB_ENGINE, INPUT_PULLUP);      // Chân đọc trạng thái động cơ
  pinMode(CB_KEY_POWER, INPUT_PULLUP);   // Chân đọc trạng thái khóa

  pinMode(RELAY_POWER_KEY_PIN, OUTPUT);  // Chân điều khiển relay nguồn khóa
  pinMode(RELAY_DAYLIGHT_PIN, OUTPUT);   // Chân điều khiển relay đèn ban ngày
  pinMode(RELAY_BEEPER_PIN, OUTPUT);     // Chân điều khiển relay còi
  pinMode(RELAY_HEADLIGHT_PIN, OUTPUT);  // Chân điều khiển relay đèn pha
  pinMode(RELAY_DC_OUT_PIN, OUTPUT);     // Chân điều khiển relay DC out

  // Thiết lập trạng thái ban đầu cho các relay
  digitalWrite(RELAY_POWER_KEY_PIN, LOW);
  digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
  digitalWrite(RELAY_BEEPER_PIN, LOW);
  digitalWrite(RELAY_HEADLIGHT_PIN, LOW);
  digitalWrite(RELAY_DC_OUT_PIN, LOW);

  // Khởi tạo nRF24L01
  SPI.begin(18, 19, 23, CSN_PIN); // SCK, MISO, MOSI, CSN
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(11);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MAX);
  Senddata(); // Gửi dữ liệu ban đầu
  radio.write(&confirm, sizeof(confirm));
  radio.startListening();

  // Khởi tạo I2C và OLED SSD1306
  Wire.begin(21, 22); // SDA, SCL
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Địa chỉ I2C của OLED

  // Khởi tạo DHT11
  dht.begin();

  // Khởi tạo MPU6050
  mpu.begin();

  // Hiệu chỉnh gyroscope
  calibrateGyro();

  // Hiển thị logo hoặc thông điệp chào mừng
  Logo(); 
  delay(2000);

  display.clearDisplay();
  display.display();
}


void loop() {
  // Phần code chính của chương trình

  // Xử lý alarmstate (Báo động)
  if (alarmstate > 0) {
    unsigned long timeCurent = millis();
    switch (alarmstate) {
      case 1: {
        // Báo động mức 1: Báo động khi khóa điện bị mở
        if (digitalRead(CB_KEY_POWER) == LOW) {
          if (timeCurent - timeSave >= 500) {
            checkwarning = !checkwarning;
            warning = 1;
            digitalWrite(RELAY_DAYLIGHT_PIN, checkwarning);
            digitalWrite(RELAY_BEEPER_PIN, checkwarning);
            timeSave = timeCurent;
            if (checkwarning) {
              Senddata();
              radio.stopListening();
              delay(200);
              radio.write(&confirm, sizeof(confirm));
              radio.startListening();
            }
          }
        } else {
          digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
          digitalWrite(RELAY_BEEPER_PIN, LOW);
          checkwarning = 0;
        }
      } break;

      case 2: {
        // Báo động mức 2: Phát hiện rung động từ MPU6050 hoặc khóa điện bị mở
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        float curgx = g.gyro.x - gyroBiasX;
        float curgy = g.gyro.y - gyroBiasY;
        float curgz = g.gyro.z - gyroBiasZ;

        int buttonState = digitalRead(CB_KEY_POWER);

        // Đặt ngưỡng cho gyroscope
        float threshold = 0.2; // Điều chỉnh giá trị này nếu cần

        // Kiểm tra điều kiện báo động
        if (buttonState == LOW || abs(curgx) > threshold || abs(curgy) > threshold || abs(curgz) > threshold) {
          if (timeCurent - timeSave >= 500) {
            checkwarning = !checkwarning;
            warning = 1;
            digitalWrite(RELAY_DAYLIGHT_PIN, checkwarning);
            digitalWrite(RELAY_BEEPER_PIN, checkwarning);
            timeSave = timeCurent;
            if (checkwarning) {
              Senddata();
              radio.stopListening();
              delay(200);
              radio.write(&confirm, sizeof(confirm));
              radio.startListening();
            }
          }
        } else {
          digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
          digitalWrite(RELAY_BEEPER_PIN, LOW);
          checkwarning = 0;
        }
      } break;

      case 3: {
        // Báo động mức 3: Phát hiện di chuyển hoặc rung động nhỏ
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        float curgx = g.gyro.x;
        float curgy = g.gyro.y;
        float curgz = g.gyro.z;

        if (digitalRead(CB_KEY_POWER) == LOW || curgx != 0.0 || curgy != 0.0 || curgz != 0.0) {
          if (timeCurent - timeSave >= 500) {
            checkwarning = !checkwarning;
            warning = 1;
            digitalWrite(RELAY_DAYLIGHT_PIN, checkwarning);
            digitalWrite(RELAY_BEEPER_PIN, checkwarning);
            timeSave = timeCurent;
            if (checkwarning) {
              Senddata();
              radio.stopListening();
              delay(200);
              radio.write(&confirm, sizeof(confirm));
              radio.startListening();
            }
          }
        } else {
          digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
          digitalWrite(RELAY_BEEPER_PIN, LOW);
          checkwarning = 0;
        }
      } break;

      default: {
        // Mức báo động không xác định
        digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
        digitalWrite(RELAY_BEEPER_PIN, LOW);
        checkwarning = 0;
      } break;
    }
  }

  // Đọc dữ liệu từ cảm biến nếu power bật
  if (power) {
    unsigned long EngineCurent = millis();

    // Đọc điện áp ắc quy
    
    battery = (VbatMax* (analogRead(CB_VOL_ACCU) / 4095.0)); // Nhân 2 nếu dùng cầu phân áp
    // Đọc nhiệt độ và độ ẩm từ DHT11
    tem = dht.readTemperature();
    if (tem > 80) tem = 80;
    if (tem < 0) tem = 0;
    hum = dht.readHumidity();
    if (hum > 99) hum = 99;
    if (hum < 10) hum = 10;

    // Đọc trạng thái khóa
    lockStatus = digitalRead(CB_KEY_POWER);

    // Cập nhật hiển thị OLED mỗi giây
    if (EngineCurent - timeRefesh >= 1000) {
      RefreshOled(); // Cập nhật hiển thị OLED
      timeRefesh = millis();
    }

    // Kiểm tra trạng thái động cơ
    if (digitalRead(CB_ENGINE) == LOW) {
      checkEngine = millis();
    } else {
      if (EngineCurent - checkEngine >= 60000) { // Nếu động cơ tắt quá 60 giây
        power = 0;
        digitalWrite(RELAY_POWER_KEY_PIN, LOW);
        digitalWrite(RELAY_DAYLIGHT_PIN, HIGH);
        Logo();
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
        digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
        delay(500);
        display.clearDisplay();
        display.display();
      }
    }
  }

  // Xử lý dữ liệu nhận được từ nRF24L01
  if (radio.available()) {
    void (*resetFunc)(void) = 0; // Hàm reset tự động

    // Đọc các cảm biến
    battery = (VbatMax * (analogRead(CB_VOL_ACCU) / 4095.0)); // Đọc điện áp ắc quy
    tem = dht.readTemperature();
    if (tem > 80) tem = 80;
    if (tem < 0) tem = 0;
    hum = dht.readHumidity();
    if (hum > 99) hum = 99;
    if (hum < 10) hum = 10;
    lockStatus = digitalRead(CB_KEY_POWER);

    char text[5] = "";
    radio.read(&text, sizeof(text));
    setting = String(text).toInt();

    switch (setting) {
      case 10: { // Kiểm tra kết nối
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
      } break;

      case 11: { // Tìm xe bằng còi và đèn
        digitalWrite(RELAY_BEEPER_PIN, HIGH);
        digitalWrite(RELAY_DAYLIGHT_PIN, HIGH);
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
        delay(100);
        digitalWrite(RELAY_BEEPER_PIN, LOW);
        delay(200);
        digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
      } break;

      case 12: { // Bật nguồn
        if (!power) Logo();
        autoReset++;
        alarmstate = 0;
        warning = 0; // Tắt báo động
        digitalWrite(RELAY_POWER_KEY_PIN, HIGH);
        digitalWrite(RELAY_DAYLIGHT_PIN, HIGH);
        power = 1;
        checkEngine = millis();
        lockStatus = digitalRead(CB_KEY_POWER);
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
        delay(200);
        digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
      } break;

      case 13: { // Tắt nguồn
        if (power) Logo();
        if (warning) {
          alarmstate = 0;
          warning = 0;
        }
        digitalWrite(RELAY_POWER_KEY_PIN, LOW);
        digitalWrite(RELAY_DAYLIGHT_PIN, HIGH);
        power = 0;
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
        delay(200);
        digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
        delay(200);
        digitalWrite(RELAY_DAYLIGHT_PIN, HIGH);
        delay(200);
        digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
        display.clearDisplay();
        display.display();
        if (autoReset >= 50 && alarmstate == 0) {
          display.setCursor(0, 10);
          display.print("Reboot...");
          display.display();
          delay(500);
          resetFunc(); // Reset tự động
        }
      } break;

      case 14: { // Tìm xe bằng còi
        lockStatus = digitalRead(CB_KEY_POWER);
        if (lockStatus == HIGH) {
          digitalWrite(RELAY_BEEPER_PIN, HIGH);
          delay(100);
          digitalWrite(RELAY_BEEPER_PIN, LOW);
          delay(200);
        } else {
          digitalWrite(RELAY_DAYLIGHT_PIN, HIGH);
          delay(200);
          digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
        }
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
      } break;

      case 15: { // Bật/tắt đèn pha
        headlight = !headlight;
        digitalWrite(RELAY_HEADLIGHT_PIN, headlight);
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
      } break;

      case 16: { // Bật/tắt nguồn DC out
        dcout = !dcout;
        digitalWrite(RELAY_DC_OUT_PIN, dcout);
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
      } break;

      case 17: { // Tắt báo động
        digitalWrite(RELAY_DAYLIGHT_PIN, LOW);
        digitalWrite(RELAY_BEEPER_PIN, LOW);
        checkwarning = 0;
        alarmstate = 0;
        warning = 0;
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
      } break;

      case 18: { // Đặt mức báo động 1
        alarmstate = 1;
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
      } break;

      case 19: { // Đặt mức báo động 2
        alarmstate = 2;
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
      } break;

      case 20: { // Đặt mức báo động 3
        alarmstate = 3;
        Senddata();
        radio.stopListening();
        delay(200);
        radio.write(&confirm, sizeof(confirm));
        radio.startListening();
      } break;

      default: {
        // Xử lý lệnh không xác định
        Serial.println("Lệnh không hợp lệ");
      } break;
    }
  }
}

