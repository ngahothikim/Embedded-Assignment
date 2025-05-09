#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define TFT_CS    6
#define TFT_DC    7
#define TFT_RST   8

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
#define ST77XX_GREY 0x5AEB // New colour

RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"5729", "2989"};

boolean power = 0, checked = 0, headlight = 0, dcout = 0, warning = 0, vibrate = 0;
int alarm = 0, accu1 = 0, accu2 = 0, pron = 0, temp = 0, hum = 0,
    sleep = 5, Menu = 0, addeeprom = 0, select, setalarm, battery, coutalarm = 0;
boolean lockStatus = 0; // 0: khóa đóng, 1: khóa mở
unsigned long timeSave = 0, timeCurent, timeWarning; byte savesleep;
unsigned long previousMillis = 0;
const long interval = 200;  // 200 ms
void setup() 
{
  Serial.begin(9600);
  pinMode(2, OUTPUT); //Power working
  pinMode(3, OUTPUT); //Motor Vibrate
  pinMode(4, OUTPUT); //Motor Vibrate
  pinMode(5, OUTPUT); //On Screen
  digitalWrite(2, HIGH); digitalWrite(5, HIGH);
  digitalWrite(3, HIGH); delay(30); digitalWrite(3, LOW);
  pinMode(A0, INPUT); //Read Battery
  pinMode(A1, INPUT_PULLUP); //Key OK
  pinMode(A2, INPUT_PULLUP); //Key Power/Menu
  pinMode(A3, INPUT_PULLUP); //Key Up
  pinMode(A4, INPUT_PULLUP); //Key Down
  if (!radio.begin()) 
  {
    Serial.println("Module không khởi động được...!!");
    while (1) 
    {}
  }
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(11);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.setPALevel(RF24_PA_MAX);
  char text[] = "10"; //Checking
  char response[20] = ""; // Biến lưu dữ liệu phản hồi từ RX
  radio.write(&text, sizeof(text));
  radio.startListening();
  delay(100);
  if (radio.available()) 
  {
    radio.read(&response, sizeof(response)); // Đọc dữ liệu từ RX
    Serial.print("Dữ liệu từ RX: ");
    Serial.println(response); // In ra dữ liệu nhận được từ RX
  } 
  else 
  {
    Serial.println("Không có phản hồi từ RX. Kết nối chưa thành công.");
  }
  savesleep = EEPROM.read(addeeprom);
  sleep = String(savesleep).toInt();
      
  tft.init(240, 240, SPI_MODE2);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
  Logo();

  delay(100);
  tft.fillScreen(ST77XX_BLACK);
  Mainmenu(); Battery();
  digitalWrite(3, HIGH); delay(30); digitalWrite(3, LOW);
}

void loop() 
{
  timeCurent = millis();

  //Coutdown Sleep
  if (timeCurent - timeSave >= sleep * 500)
  {
    if(alarm < 3)
    {
      Serial.println("i sleep");
      tft.fillScreen(ST77XX_BLACK);
      Logo();
      delay(600);
      digitalWrite(2, LOW);
      //
      digitalWrite(5, LOW);
      digitalWrite(4, HIGH);
      delay(200);
      digitalWrite(4, LOW);
      delay(200);
    } 
    else
    {
      if(analogRead(A0) < 550) 
      {
        digitalWrite(3, HIGH); delay(50); digitalWrite(2, LOW);
      }
      if(Menu != 2) tft.fillScreen(ST77XX_BLACK);
      digitalWrite(5, LOW); Menu = 2;
    }
  }

  //Warning
  if(warning || coutalarm != 0){
    Serial.println("hello warning");
    if(timeCurent - timeWarning >= 40){
      vibrate = !vibrate; digitalWrite(3, vibrate); coutalarm++; timeWarning = timeCurent;
      if(coutalarm >= 4){
        digitalWrite(3, LOW); warning = 0; coutalarm = 0; vibrate = 0;
      }
    }
  }

  
  //Main menu
  if(Menu == 0)
  {
    Serial.println("hello main menu");
    if(digitalRead(A1) == HIGH)
    {
      radio.stopListening();
      char text[] = "11"; //Find Light
      radio.write(&text, sizeof(text));
      radio.startListening();
      checked = 0; Checking(); Battery();
      digitalWrite(4, HIGH);
      delay(200);
      digitalWrite(4, LOW);
      delay(200);
    }
    else if(digitalRead(A3) == HIGH)
    {
      Serial.println("main menu power on");
      radio.stopListening();
      char text[] = "12"; //Power ON
      radio.write(&text, sizeof(text));
      radio.startListening();
      checked = 0; Checking(); Battery();
      digitalWrite(3, LOW); warning = 0; coutalarm = 0; vibrate = 0;
      digitalWrite(4, HIGH);
      delay(200);
      digitalWrite(4, LOW);
      delay(200);
      //delay(200);
    }
    else if(digitalRead(A4) == HIGH)
    {
      radio.stopListening();
      char text[] = "13"; //Power OFF
      radio.write(&text, sizeof(text));
      radio.startListening();
      checked = 0; Checking(); Battery();
      digitalWrite(3, LOW); warning = 0; coutalarm = 0; vibrate = 0;
            digitalWrite(4, HIGH);
      delay(200);
      digitalWrite(4, LOW);
      delay(200);
      //delay(200);
    }
    else if(digitalRead(A2) == HIGH)
    {
      digitalWrite(5, HIGH);
      Serial.println("main menu to sub");
      Menu = 1;
      tft.fillScreen(ST77XX_BLACK);
      Submenu(); Battery();
            digitalWrite(4, HIGH);
      delay(200);
      digitalWrite(4, LOW);
      delay(200);
      digitalWrite(5, HIGH);
      //delay(200);
    }
  }
  else if(Menu == 1)
  {
    if(digitalRead(A1) == HIGH)
    {
      Serial.println("hello sub menu");
      tft.drawRoundRect(0, select, 240, 40, 8, ST77XX_RED);
      switch(select)
      {
        case 35: 
        {
          Serial.println("beep beep");
          radio.stopListening();
          char text[] = "14"; //lock door
          radio.write(&text, sizeof(text));
          radio.startListening();
          checked = 0; Checking();
          delay(400); tft.drawRoundRect(0, select, 240, 40, 8, ST77XX_BLUE);
        } break;
        case 85: 
        {
          Serial.println("hello menu 12");
          Menu = 12; break;
        }
        case 135:
        {
          Serial.println("hello menu 13");
          Menu = 13; setalarm = alarm; break;
        } 
        case 185: 
        {
          Serial.println("hello menu 14");
          Menu = 14; break;
        }
      }
      delay(200); Battery();
    }
    if(digitalRead(A2) == HIGH)
    {
      Serial.println("back to main menu");
      Menu = 0;
      tft.fillScreen(ST77XX_BLACK);
      Mainmenu(); Battery();
      delay(200);
    }
    if(digitalRead(A4) == HIGH)
    {
      Serial.println("go down");
      if(select < 185){
        tft.drawRoundRect(0, select, 240, 40, 8, ST77XX_WHITE);
        select += 50;
        tft.drawRoundRect(0, select, 240, 40, 8, ST77XX_BLUE);
      }
      delay(200); Battery();
    }
    if(digitalRead(A3) == HIGH)
    {
      Serial.println("go up");
      if(select > 35){
        select -= 50;
        tft.drawRoundRect(0, select, 240, 40, 8, ST77XX_BLUE);
        tft.drawRoundRect(0, select + 50, 240, 40, 8, ST77XX_WHITE);
      }
      delay(200); Battery();
    }
  }
  else if(Menu == 2)
  { //Menu Screen ON/OFF
      if(digitalRead(A2) == HIGH)
      {
        Serial.println("back to main menu");
        digitalWrite(5, HIGH);
        tft.fillScreen(ST77XX_BLACK);
        Menu = 0; Mainmenu(); Battery();
        delay(200);
      }
  }
  else if(Menu == 12)
  {//Headlight/DC out
    Serial.println("hello headlight menu");
    if(digitalRead(A3) == HIGH)
    {
      Serial.println("sáng đèn đầu");
      radio.stopListening();
      char text[] = "15"; //Head Light
      radio.write(&text, sizeof(text));
      radio.startListening();
      checked = 0; Checking(); Battery();
      delay(200);
    }
    else if(digitalRead(A4) == HIGH)
    {
      Serial.println("DC out");
      radio.stopListening();
      char text[] = "16"; //DC out
      radio.write(&text, sizeof(text));
      radio.startListening();
      checked = 0; Checking(); Battery();
      delay(200);
    }
    else if(digitalRead(A1) == HIGH)
    {
      tft.drawRoundRect(0, select, 240, 40, 8, ST77XX_BLUE);
      Menu = 1; Battery();
      delay(200);
    }
    else if(digitalRead(A2) == HIGH)
    {
      Menu = 0;
      tft.fillScreen(ST77XX_BLACK);
      Mainmenu(); Battery();
      delay(200);
    }
  }
  else if(Menu == 13)
  { //Alarm level
    Serial.println("hello Alarm menu");
    if(digitalRead(A3) == HIGH)
    { // Alarm +
      Serial.println("Alarm+");
      if(setalarm < 3) 
      {
        setalarm++;
      }
      switch(setalarm)
      {
        case 1: 
        { 
          radio.stopListening();
          char text[] = "18"; //Set alarm = 1
          radio.write(&text, sizeof(text));
          radio.startListening(); checked = 0; Checking(); 
        } 
        break;
        case 2: 
        { 
          radio.stopListening();
          char text[] = "19"; //Set alarm = 2
          radio.write(&text, sizeof(text));
          radio.startListening(); checked = 0; Checking(); 
        } 
        break;
        case 3: 
        { 
          radio.stopListening();
          char text[] = "20"; //Set alarm = 3
          radio.write(&text, sizeof(text));
          radio.startListening(); checked = 0; Checking(); 
        } 
        break;
      }
      delay(200); Battery();
    }
    else if(digitalRead(A4) == HIGH)
    { //Alarm -
      Serial.println("alarm -");
      if(setalarm > 0)
      {
        setalarm--;
      } 
      switch(setalarm)
      {
        case 0: 
        { 
          radio.stopListening();
          char text[] = "17"; //Set alarm = 0
          radio.write(&text, sizeof(text));
          radio.startListening(); checked = 0; Checking(); 
        } 
        break;
        case 1: 
        { 
          radio.stopListening();
          char text[] = "18"; //Set alarm = 1
          radio.write(&text, sizeof(text));
          radio.startListening(); checked = 0; Checking(); 
        } 
        break;
        case 2: 
        { radio.stopListening();
          char text[] = "19"; //Set alarm = 2
          radio.write(&text, sizeof(text));
          radio.startListening(); checked = 0; Checking(); 
        } 
        break;
      }
      delay(200); Battery();
    }
    else if(digitalRead(A1) == HIGH)
    {
      Serial.println("alarm back to sub");
      tft.drawRoundRect(0, select, 240, 40, 8, ST77XX_BLUE);
      Menu = 1; Battery();
      delay(200);
    }
    else if(digitalRead(A2) == HIGH)
    {
      Serial.println("alarm back to main");
      Menu = 0;
      tft.fillScreen(ST77XX_BLACK);
      Mainmenu(); Battery();
      delay(200);
    }
  }
  else if(Menu == 14)
  {//Sleep timer
    Serial.println("hello sleep timer");
    if(digitalRead(A3) == HIGH)
    {
      if(sleep < 30) sleep += 5;
      tft.fillRoundRect(170, 190, 40, 30, 7, ST77XX_GREY);
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(179, 198);
      tft.print(sleep); Battery();
      delay(200);
    }
    else if(digitalRead(A4) == HIGH)
    {
      if(sleep > 5) sleep -= 5;
      tft.fillRoundRect(170, 190, 40, 30, 7, ST77XX_GREY);
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(186, 198);
      if(sleep > 5) tft.setCursor(179, 198);
      tft.print(sleep); Battery();
      delay(200);
    }
    else if(digitalRead(A1) == HIGH)
    {
      Serial.println("back to sub menu");
      tft.drawRoundRect(0, select, 240, 40, 8, ST77XX_BLUE);
      EEPROM.write(addeeprom, sleep);
      Menu = 1; Battery();
      delay(200);
    }
    else if(digitalRead(A2) == HIGH)
    {
      Serial.println("back to main menu");
      Menu = 0;
      tft.fillScreen(ST77XX_BLACK);
      Mainmenu(); Battery();
      delay(200);
    }
  }
  //Reciverd

  if (radio.available()) {
    Serial.println("có radio");
      char text[20], HeadLight[3], DcOut[3], Power[3], Alarm[3], Accu1[5], Accu2[3],
           LockStatus[2], Temp[5], Hum[5], Warning[3];
      radio.read(&text, sizeof(text));
        strncpy(HeadLight, text, 1);
        HeadLight[1] = '\0';
        strncpy(DcOut, text+1, 1);
        DcOut[1] = '\0';
        strncpy(Power, text+2, 1);
        Power[1] = '\0';
        strncpy(Alarm, text+3, 1);
        Alarm[1] = '\0';
        strncpy(Accu1, text+4, 2);
        Accu1[2] = '\0';
        strncpy(Accu2, text+6, 1);
        Accu2[1] = '\0';
        strncpy(LockStatus, text + 7, 1); // Thêm dòng này
        LockStatus[1] = '\0';
        strncpy(Temp, text+10, 2);
        Temp[2] = '\0';
        strncpy(Hum, text+12, 2);
        Hum[2] = '\0';
        strncpy(Warning, text+14, 1);
        Warning[1] = '\0';
       headlight = String(HeadLight).toInt();
       dcout = String(DcOut).toInt();
       power = String(Power).toInt();
       alarm = String(Alarm).toInt();
       accu1 = String(Accu1).toInt() -10;
       accu2 = String(Accu2).toInt();
       lockStatus = String(LockStatus).toInt(); // Chuyển đổi giá trị lockStatus
       temp = String(Temp).toInt() -10;
       hum = String(Hum).toInt();
       warning = String(Warning).toInt();
      checked = 1;
      if(Menu == 13){
        tft.fillRoundRect(170, 140, 40, 30, 7, ST77XX_GREY);
        tft.setTextColor(ST77XX_GREEN);
        if(alarm > 2) tft.setTextColor(ST77XX_RED);
        tft.setCursor(186, 148);
        tft.print(alarm);
      }
      if(Menu == 0) Updatemenu();
      Checking(); Attention();
      if(Menu == 2){
        digitalWrite(5, HIGH); delay(500);
        WarningAlarm();
        timeSave = timeCurent;
      }
  }
}

