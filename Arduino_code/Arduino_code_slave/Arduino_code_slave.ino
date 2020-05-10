#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial MasterSerial(12,13); // Rx , Tx

LiquidCrystal_I2C lcd(0x27,16,4); 

String Master_comm = "";
String To_Master_comm = "";

void setup() {

  Serial.begin(9600);
  MasterSerial.begin(9600);

  lcd.init();
  lcd.backlight();
  //lcd.print("Hello World!");
  lcd.noCursor();

}

void loop() {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.print(millis() / 1000);

  while(MasterSerial.available()){

    delay(100);
    char c = MasterSerial.read();
    Master_comm += c;
    
  }
  if (Master_comm.length() != 0){
    Serial.println(Master_comm);
    lcd.print(Master_comm);
    delay(1000);
  }
  Master_comm = "";
  while(Serial.available()){
    
    delay(10);
    char c = Serial.read();
    To_Master_comm += c;
    
    
  }
 // Serial.println(To_Master_comm);
  MasterSerial.print(To_Master_comm);
  To_Master_comm = "";
  

  delay(100);

}
