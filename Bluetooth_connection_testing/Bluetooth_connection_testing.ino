#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial BTSerial(0,1); // RX, TX
//LiquidCrystal lcd();

String comm;
int LED_pin = 3;
int Night_pin = 4;
int Night_in = A4;
int Therm_in = A5;
int Temperature;

void setup() {

  Serial.begin(9600);
  BTSerial.begin(9600);
  
  comm = "";
  
  pinMode(LED_pin, OUTPUT);
  pinMode(Night_pin, OUTPUT);
  pinMode(Night_in, INPUT);
  pinMode(Therm_in, INPUT);

}

void loop() {
  
  while(Serial.available()){

    delay(10);
    char c = Serial.read();
    comm += c;
    
  }
  if(comm == "on"){

    digitalWrite(LED_pin, HIGH);
    
  }
  else if(comm == "off"){

    digitalWrite(LED_pin,LOW);
    
  }
  
  Serial.print(comm);
  comm = "";

  float analog_read = (analogRead(Therm_in) / 204.6);

  if(analog_read >= 0.1){
    
    Temperature = ((analog_read - 0.5) * 100);
    //Serial.println(Temperature);
    //BTSerial.print(Temperature);
    
  }

  float night_read = (analogRead(Night_in) / 1024);
  Serial.println(night_read);

  //if(analog_read <= (2/3)){

    //digitalWrite(Night_pin, HIGH); 

  //}
  //else{

    //digitalWrite(Night_pin, LOW);
    
  //}
  
}
