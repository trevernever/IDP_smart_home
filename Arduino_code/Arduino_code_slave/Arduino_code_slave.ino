//#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

//SoftwareSerial MasterSerial(13,12) // Rx , Tx

LiquidCrystal_I2C lcd(0x27,16,4); 

void setup() {

  Serial.begin(9600);
  //MasterSerial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.print("Hello World!");
  lcd.noCursor();

}

void loop() {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(millis() / 1000);

  delay(10);

}
