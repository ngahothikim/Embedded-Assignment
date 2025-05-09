void Senddata(){
  accu = battery * 10;
  datasend = String(headlight) + String(dcout) + String(power) + String(alarmstate)
            + String(accu + 100) + String(lockStatus) + String(tem + 10) + String(hum) + String(warning);
  int len = datasend.length() + 1;
  datasend.toCharArray(confirm, len);
}

void Logo(){
  display.clearDisplay();
  display.fillTriangle(3, 5, 16, 10, 16, 31, SSD1306_WHITE);
  display.drawLine(3, 5, 16, 0, SSD1306_WHITE);
  display.drawLine(17, 0, 30, 5, SSD1306_WHITE);
  display.drawLine(30, 5, 17, 31, SSD1306_WHITE);
  display.drawLine(18, 9, 19, 9, SSD1306_WHITE);
  display.drawLine(19, 9, 19, 11, SSD1306_WHITE);
  display.drawLine(21, 8, 21, 10, SSD1306_WHITE);
  display.drawLine(22, 9, 23, 8, SSD1306_WHITE);
  display.drawPixel(23, 10, SSD1306_WHITE);
  display.drawPixel(26, 6, SSD1306_WHITE);
  display.drawPixel(25, 7, SSD1306_WHITE);
  display.drawPixel(26, 8, SSD1306_WHITE);
  display.drawPixel(25, 9, SSD1306_WHITE);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(40, 2);
  display.println("POWER BY");
  display.setTextSize(2);
  display.setCursor(40, 14);
  display.println("TAN AN");
  display.display();
}

void RefreshOled(){
  int color = SSD1306_BLACK;
  if(digitalRead(CB_ENGINE) == LOW) color = SSD1306_WHITE; // Thay digitalRead(7) bằng CB_ENGINE
  display.fillRect(33, 0, 94, 31, SSD1306_BLACK);
    display.drawCircle(38, 15, 4, color);
    display.drawLine(36, 9, 40, 9, color);
    display.drawLine(32, 13, 32, 17, color);
    display.drawLine(44, 13, 44, 17, color);
    display.drawPixel(38, 10, color);
    display.drawPixel(33, 15, color);
    display.drawPixel(43, 15, color);

    display.drawRect(48, 3, 13, 10, SSD1306_WHITE);
    display.drawLine(50, 2, 51, 2, SSD1306_WHITE);
    display.drawLine(57, 2, 58, 2, SSD1306_WHITE);
    display.drawLine(50, 7, 58, 7, SSD1306_WHITE);
    display.drawLine(50, 10, 58, 10, SSD1306_WHITE);

    display.drawCircle(54, 26, 4, SSD1306_WHITE);
    display.fillRect(53, 17, 3, 5, SSD1306_WHITE);
    display.drawPixel(55, 17, SSD1306_BLACK);
    display.drawPixel(55, 19, SSD1306_BLACK);
    display.drawPixel(55, 21, SSD1306_BLACK);
  display.setTextSize(2);
  display.setCursor(68, 0);
  display.print(battery);
  display.setCursor(68, 16);
  display.print(tem);
  display.print("|");
  display.print(hum);
  display.display();
}

void calibrateGyro() {
  float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
  sensors_event_t a, g, temp;

  Serial.println("Đang hiệu chỉnh cảm biến gyroscope... Vui lòng giữ yên cảm biến.");

  for(int i = 0; i < calibrationSamples; i++) {
    mpu.getEvent(&a, &g, &temp);
    sumX += g.gyro.x;
    sumY += g.gyro.y;
    sumZ += g.gyro.z;
    delay(10); // Chờ một chút giữa các mẫu
  }

  gyroBiasX = sumX / calibrationSamples;
  gyroBiasY = sumY / calibrationSamples;
  gyroBiasZ = sumZ / calibrationSamples;

  Serial.println("Hiệu chỉnh hoàn tất.");
  Serial.print("Gyro Bias X: "); Serial.println(gyroBiasX);
  Serial.print("Gyro Bias Y: "); Serial.println(gyroBiasY);
  Serial.print("Gyro Bias Z: "); Serial.println(gyroBiasZ);
}
