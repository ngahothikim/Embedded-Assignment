void Submenu(){
  int color = ST77XX_GREY;
  if(checked) color = ST77XX_WHITE;
  Checking();
  Attention();
  int i = 0;
  for(int y = 35; y <= 185; y += 50){
    i++;
    tft.drawRoundRect(0, y, 240, 40, 8, ST77XX_WHITE);
    if(i > 1){
      tft.drawTriangle(215, y + 16, 223, y + 5, 231, y + 16, ST77XX_GREEN);
      tft.drawTriangle(215, y + 24, 223, y + 35, 231, y + 24, ST77XX_GREEN);
    }
    if(i > 2) tft.fillRoundRect(170, y + 5, 40, 30, 7, ST77XX_GREY);
  }
  tft.setTextColor(color);
  tft.setTextSize(2);
  tft.setCursor(10, 48);
  tft.print("Lock Door");
  tft.setCursor(10, 98);
  tft.print("Light/DC Out");
  tft.setCursor(10, 148);
  tft.print("Alarm Level");
  tft.setCursor(10, 198);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("Sleep Timer");
    tft.setTextColor(ST77XX_GREEN);
    if(alarm > 2) tft.setTextColor(ST77XX_RED);
    tft.setCursor(186, 148);
    tft.print(alarm);
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(186, 198);
    if(sleep > 5) tft.setCursor(179, 198);
    tft.print(sleep);
  tft.drawRoundRect(0, 35, 240, 40, 8, ST77XX_BLUE);
  select = 35;
}
