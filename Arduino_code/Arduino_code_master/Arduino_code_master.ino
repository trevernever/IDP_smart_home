#include <SoftwareSerial.h>

SoftwareSerial BTSerial(0,1); // RX , TX
SoftwareSerial SlaveSerial(12,13); // RX , TX

String BT_comm;
String Slave_comm;
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
  
  while(BTSerial.available()){

    delay(10);
    char c = Serial.read();
    BT_comm += c;
    
  }
  
  while(SlaveSerial.available()){

    delay(10);
    char c = Serial.read();
    Slave_comm += c;
    
  }
  
  if(BT_comm == "on"){

    digitalWrite(LED_pin, HIGH);
    
  }
  else if(BT_comm == "off"){

    digitalWrite(LED_pin,LOW);
    
  }
  
  Serial.print(BT_comm);
  BT_comm = "";

  float analog_read = (analogRead(Therm_in) / 204.6);

  if(analog_read >= 0.1){
    
    Temperature = ((analog_read - 0.5) * 100);
    //Serial.println(Temperature);
    //BTSerial.print(Temperature);
    
  }

  int night_read = analogRead(Night_in);
  int light_threshold = 642;
  //Serial.println(night_read);

  if(night_read < light_threshold){

    digitalWrite(Night_pin, HIGH); 

  }
  else{

    digitalWrite(Night_pin, LOW);
    
  }
  
}
