void Power(){
  int color = ST77XX_WHITE;
  if(power) color = ST77XX_GREEN;
  if(!checked) color = ST77XX_GREY;
    tft.fillCircle(21, 62, 10, color);
    tft.fillCircle(15, 62, 2, ST77XX_BLACK);
    tft.fillTriangle(31, 58, 41, 58, 36, 66, color);
    tft.fillTriangle(41, 58, 51, 58, 46, 66, color);
  tft.setTextColor(color);
  tft.setCursor(67, 58);
  if(power){tft.print("ON");
  } else{
    tft.print("OFF");
  }
}
void Accu(){
  int color = ST77XX_WHITE;
  if(accu1 > 0) color = ST77XX_RED;
  if(accu1 >= 10) color = ST77XX_YELLOW;
  if(accu1 >= 12) color = ST77XX_GREEN;
  if(!checked) color = ST77XX_GREY;
    tft.drawRect(138, 55, 30, 20, color);
    tft.drawRect(139, 56, 28, 18, color);
    tft.fillRect(142, 51, 3, 2, color);
    tft.fillRect(161, 51, 3, 2, color);
    tft.drawLine(144, 65, 160, 65, color);
  tft.setTextColor(color);
  tft.setCursor(175, 58);
  tft.print(accu1); tft.print(".");tft.print(accu2); tft.print("V");
}
void Output(){
  int color1 = ST77XX_WHITE, color2 = ST77XX_WHITE;
  if(headlight) color1 = ST77XX_BLUE;
  if(!checked) color1 = ST77XX_GREY;
    tft.fillCircle(30, 138, 14, color1);
    tft.fillRect(15, 124, 30, 14, ST77XX_BLACK);
    tft.drawLine(30, 135, 30, 126, color1);
    tft.drawLine(26, 135, 23, 126, color1);
    tft.drawLine(21, 135, 16, 126, color1);
    tft.drawLine(34, 135, 37, 126, color1);
    tft.drawLine(39, 135, 44, 126, color1);
  if(dcout) color2 = ST77XX_RED;
  if(!checked) color2 = ST77XX_GREY;
    tft.fillCircle(85, 138, 14, color2);
    tft.fillCircle(85, 138, 11, ST77XX_BLACK);
    tft.fillCircle(85, 138, 9, color2);
    tft.fillRect(84, 125, 3, 14, ST77XX_BLACK);
    tft.drawRect(84, 124, 5, 16, color2);
}

void LockDoor(){
  int colorunlock = ST77XX_WHITE;
  int colorlock = ST77XX_WHITE;
  if(!checked)
  {
    colorunlock = ST77XX_GREY;
    colorlock = ST77XX_GREY;
  } 
  if(lockStatus==1)
  {
    colorlock = ST77XX_BLUE;
    // Xóa vùng hiển thị cũ
    tft.fillRect(130, 120, 105, 42, ST77XX_BLACK);

    tft.fillRoundRect(138, 140, 30, 20, 5, colorlock);

    // Vẽ vòng cung mở của ổ khóa
    //tft.drawCircle(153, 130, 15, color);
    //tft.fillRoundRect(153, 130, 25, 15, 7, color);
    // Vẽ lỗ khóa (nếu cần)
    tft.drawRoundRect(141, 125, 25, 25, 7, colorlock);
    tft.fillCircle(153, 150, 3, ST77XX_BLACK);
    tft.setCursor(175, 140);
    tft.setTextSize(2);
    tft.setTextColor(colorlock);
    tft.print("Lock");
   
  } 
  if(lockStatus==0)
  {
    colorunlock = ST77XX_RED;
    tft.fillRect(130, 120, 105, 42, ST77XX_BLACK);
    tft.fillRoundRect(135, 140, 30, 20, 5, colorunlock);

    // Vẽ vòng cung mở của ổ khóa
    //tft.drawCircle(153, 130, 15, color);
    //tft.fillRoundRect(153, 130, 25, 15, 7, color);
    // Vẽ lỗ khóa (nếu cần)
    tft.drawRoundRect(138, 125, 25, 25, 7, colorunlock);
    tft.fillRoundRect(157, 133, 10, 7, 1, ST77XX_BLACK);
    tft.fillCircle(150, 150, 3, ST77XX_BLACK);
    tft.setCursor(168, 140);
    tft.setTextSize(2);
    tft.setTextColor(colorunlock);
    tft.print("Unlock");
  }
}

void Pron(){
  int color = ST77XX_WHITE;

  if(!checked) color = ST77XX_GREY;
    tft.drawRoundRect(138, 120, 30, 15, 3, color);
    tft.drawRoundRect(139, 121, 28, 13, 2, color);
    tft.drawRect(144, 124, 18, 6, color);
    tft.fillRect(138, 134, 30, 20, color);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(148, 135);
  tft.print("P");
  tft.setTextColor(color);
  tft.setCursor(180, 132);
  tft.print(pron); tft.print("%");
}
void Alarm(){
  int color = ST77XX_WHITE;
  switch(alarm){
    case 1: color = ST77XX_GREEN; break;
    case 2: color = ST77XX_YELLOW; break;
    case 3: color = ST77XX_RED; break;
  }
  if(!checked) color = ST77XX_GREY;
    tft.fillTriangle(16, 225, 31, 200, 46, 225, color);
    tft.fillTriangle(21, 222, 31, 206, 41, 222, ST77XX_BLACK);
    tft.drawLine(31, 214, 31, 220, color);
  tft.setTextColor(color);
  tft.setCursor(73, 210);
  tft.print(alarm);
}
void Temp(){
  int color = ST77XX_WHITE;
  if(temp > 0) color = ST77XX_BLUE;
  if(temp > 29) color = ST77XX_RED;
  if(!checked) color = ST77XX_GREY;
    tft.drawRoundRect(150, 195, 6, 18, 3, color);
    tft.fillCircle(153, 220, 10, color);
    tft.drawPixel(157, 199, color);
    tft.drawPixel(157, 203, color);
    tft.drawPixel(157, 207, color);
  tft.setTextColor(color);
  tft.setCursor(172, 210);
  tft.print(temp); tft.print("|");tft.print(hum);
}
void Mainmenu(){
  Checking();
  Attention();
  for(int x = 0; x <= 125; x += 125){
    for(int y = 30; y <= 180; y += 75){
      tft.drawRoundRect(x, y, 115, 60, 7, ST77XX_WHITE);
      tft.fillRect(x + 22, y - 2, 70, 4, ST77XX_BLACK);
    }
  }
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 25);
  tft.print("Power");
  tft.setCursor(159, 25);
  tft.print("Accu");
  tft.setCursor(40, 100);
  tft.print("Out");
  tft.setCursor(157, 100);
  tft.print("Door");
  //tft.print("P-Ron");
  tft.setCursor(29, 175);
  tft.print("Alarm");
  tft.setCursor(159, 175);
  tft.print("Temp");
  Power(); Accu(); Output(); LockDoor(); Alarm(); Temp();
}
void Updatemenu(){
  tft.fillRect(60, 55, 47, 20, ST77XX_BLACK);
  tft.fillRect(173, 55, 66, 20, ST77XX_BLACK);
  tft.fillRect(60, 129, 47, 20, ST77XX_BLACK);
  tft.fillRect(173, 129, 66, 20, ST77XX_BLACK);
  tft.fillRect(60, 207, 47, 20, ST77XX_BLACK);
  tft.fillRect(171, 207, 68, 20, ST77XX_BLACK);
  Power(); Accu(); Output(); LockDoor()/*Pron()*/; Alarm(); Temp();
}
