#include <SoftwareSerial.h>

SoftwareSerial BTSerial(0,1); // RX , TX
SoftwareSerial SlaveSerial(12,13); // RX , TX

String BT_comm;
String Slave_comm;
int LED_pin = 3;
int Night_pin = 4;
int Night_in = A4;
int Therm_in = A5;
int Motion_in = A2;
int Temperature = 0;
int oldTemp = 0;
bool armed = false;

void setup() {

  Serial.begin(9600);
  BTSerial.begin(9600);
  
  pinMode(LED_pin, OUTPUT);
  pinMode(Night_pin, OUTPUT);
  pinMode(Night_in, INPUT);
  pinMode(Therm_in, INPUT);
  pinMode(Motion_in, INPUT);

}

void loop() {
  
  while(BTSerial.available()){

    delay(10);
    char c = BTSerial.read();
    BT_comm += c;
    
  }
  
  //while(SlaveSerial.available()){

    //delay(10);
   // char c = Serial.read();
    //Slave_comm += c;
    
  //}
  
  if(BT_comm == "on"){
    Serial.println(BT_comm);
    digitalWrite(LED_pin, HIGH);
    
  }
  else if(BT_comm == "off"){
    Serial.println(BT_comm);
    digitalWrite(LED_pin,LOW);
    
  }
  
  //Serial.print(BT_comm);
  BT_comm = "";

  float analog_read = (analogRead(Therm_in) / 204.6);

  if(analog_read >= 0.1){
    
    Temperature = (int)((analog_read - 0.5) * 100);
    //Serial.println(Temperature);
    if(Temperature != oldTemp){

      oldTemp = Temperature;
      String ret = String(Temperature,0) + "C";
      BTSerial.print(ret);
      
    }
    
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


  if(armed){

    int motion_read = analogRead(Motion_in);
    //Serial.println(motion_read);
    if(motion_read > 660){

      //sound alarm
       
    }
    
  }

  delay(100);
  
}
