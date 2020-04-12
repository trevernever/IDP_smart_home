//#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial MasterSerial(13,12) // Rx , Tx

LiquidCrystal_I2C lcd(0x27,16,4); 

String Master_comm = "";
String To_Master_comm = "";

void setup() {

  Serial.begin(9600);
  MasterSerial.begin(9600);

  //lcd.init();
  //lcd.backlight();
  //lcd.print("Hello World!");
  //lcd.noCursor();

}

void loop() {
  
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print(millis() / 1000);

  while(MasterSerial.available()){

    delay(10);
    char c = MasterSerial.read();
    Master_comm += c;
    
  }
  Serial.println(Master_comm);

  while(Serial.available()){

    delay(10);
    char c = Serial.read();
    To_Master_comm += c;
    
  }
  MasterSerial.println(To_Master_comm);

  delay(10);

}
