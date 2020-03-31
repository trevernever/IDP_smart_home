//#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//SoftwareSerial MasterSerial(13,12) // Rx , Tx

LiquidCrystal lcd(0,1,2,3,4,5,6,7,8,9); 
// Rs, Enable, d0, d1, d2, d3, d4, d5, d6, d7

//int Rs = 0;
//int Enable = 1;
//int d0 = 2;
//int d1 = 3;
//int d2 = 4;
//int d3 = 5;
//int d4 = 6;
//int d5 = 7;
//int d6 = 8;
//int d7 = 9;

void setup() {

  Serial.begin(9600);
  //MasterSerial.begin(9600);

  //pinMode(d0,OUTPUT);
  //pinMode(d1,OUTPUT);
  //pinMode(d2,OUTPUT);
  //pinMode(d3,OUTPUT);
  //pinMode(d4,OUTPUT);
  //pinMode(d5,OUTPUT);
  //pinMode(d6,OUTPUT);
  //pinMode(d7,OUTPUT);
  //pinMode(Rs,OUTPUT);
  //pinMode(Enable,OUTPUT);

  lcd.begin(20,2);
  lcd.print("Hello World!");
  //lcd.clear();
  //lcd.noCursor();

}

void loop() {
  
  //lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);

}
