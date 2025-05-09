

void Logo() {
  
   // Thiết lập màu sắc
  uint16_t bodyColor = tft.color565(169, 169, 169); // Màu xám cho thân robot
  uint16_t eyeColor = ST77XX_BLACK;
  uint16_t antennaColor = tft.color565(255, 0, 0); // Màu đỏ cho ăng-ten
  uint16_t detailColor = ST77XX_BLACK;

  // Tọa độ trung tâm
  int16_t centerX = tft.width() / 2;
  int16_t centerY = tft.height() / 2 - 20; // Di chuyển lên để có chỗ cho tên phía dưới

  // Vẽ đầu robot
  tft.fillRect(centerX - 30, centerY - 60, 60, 50, bodyColor);
  tft.drawRect(centerX - 30, centerY - 60, 60, 50, detailColor);

  // Vẽ ăng-ten
  tft.drawLine(centerX, centerY - 60, centerX, centerY - 80, antennaColor);
  tft.fillCircle(centerX, centerY - 85, 5, antennaColor);

  // Vẽ mắt
  tft.fillRect(centerX - 20, centerY - 50, 15, 10, eyeColor);
  tft.fillRect(centerX + 5, centerY - 50, 15, 10, eyeColor);

  // Vẽ miệng
  tft.drawRect(centerX - 15, centerY - 30, 30, 5, detailColor);

  // Vẽ thân robot
  tft.fillRect(centerX - 40, centerY - 10, 80, 60, bodyColor);
  tft.drawRect(centerX - 40, centerY - 10, 80, 60, detailColor);

  // Vẽ tay
  tft.fillRect(centerX - 50, centerY - 5, 10, 40, bodyColor);
  tft.fillRect(centerX + 40, centerY - 5, 10, 40, bodyColor);

  // Vẽ chân
  tft.fillRect(centerX - 30, centerY + 50, 20, 30, bodyColor);
  tft.fillRect(centerX + 10, centerY + 50, 20, 30, bodyColor);

  // Vẽ các chi tiết trên thân
  tft.fillRect(centerX - 15, centerY + 5, 30, 10, detailColor);
  tft.drawLine(centerX - 15, centerY + 15, centerX + 15, centerY + 15, detailColor);

  // Hiển thị tên "TAN AN HCMUTE"
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  int16_t x1_text, y1_text;
  uint16_t w, h;
  tft.getTextBounds("TAN AN HCMUTE", 0, 0, &x1_text, &y1_text, &w, &h);
  tft.setCursor(centerX - w / 2, tft.height() - h - 10);
  tft.print("TAN AN HCMUTE");
  
}

