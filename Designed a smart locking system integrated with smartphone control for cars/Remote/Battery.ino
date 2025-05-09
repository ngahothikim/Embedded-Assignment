

void Battery(){
  timeSave = millis();
  battery = map(analogRead(A0), 525, 610, 0, 100);// value = 520 - 640
  if(battery < 0) battery = 0;
  if(battery > 100) battery = 100;
  int color, w, h;
  if(battery > 65){
    color = ST77XX_GREEN;
    w = 18; h = 14;
  } else{
    if(battery > 30){
      color = ST77XX_YELLOW;
      w = 10; h = 14;
    } else{
      color = ST77XX_RED;
      w = 5; h = 14;
    }
  }

  tft.fillRect(166, 0, 74, 20, ST77XX_BLACK);
  if(battery >= 100) {
    tft.setCursor(167, 3);
  } else {
      if(battery >= 10){
        tft.setCursor(179, 3);
      } else{
          tft.setCursor(191, 3);
        }
    }
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.print(int(battery)); tft.print("%");
  tft.drawLine(236, 8, 236, 12, ST77XX_WHITE);
  tft.drawRoundRect(215, 2, 20, 16, 3, ST77XX_WHITE);
  tft.fillRoundRect(216, 3, w, h, 2, color);
  if(battery < 10){
    digitalWrite(3, HIGH); delay(20); digitalWrite(3, LOW);
  }
}
