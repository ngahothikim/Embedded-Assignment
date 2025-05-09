void Checking()
{
  tft.fillRect(0, 0, 15, 20, ST77XX_BLACK);
  int color1 = ST77XX_GREY, color2 = ST77XX_GREY;
  if(checked) { color1 = ST77XX_GREEN; color2 = ST77XX_BLUE;}
  tft.fillRoundRect(6, 5, 4, 13, 2, color1);
  tft.fillTriangle(8, 2, 4, 7, 12, 7, color1);
  tft.fillRoundRect(15, 2, 4, 13, 2, color2);
  tft.fillTriangle(17, 18, 13, 13, 21, 13, color2);
}
void Attention(){
  if(warning) {
    tft.fillTriangle(37, 1, 27, 18, 47, 18, ST77XX_RED);
    tft.drawLine(37, 6, 37, 13, ST77XX_BLACK);
    tft.drawPixel(37, 15, ST77XX_BLACK);
  } else {
    tft.fillRect(26, 0, 22, 20, ST77XX_BLACK);
  }
}
void WarningAlarm(){
  tft.fillTriangle(120, 47, 37, 184, 203, 184, ST77XX_RED);
  tft.fillRoundRect(112, 89, 17, 59, 5, ST77XX_BLACK);
  tft.fillRoundRect(112, 156, 17, 13, 5, ST77XX_BLACK);
}
