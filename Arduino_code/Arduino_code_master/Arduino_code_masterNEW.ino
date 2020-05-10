////////////////////////////////////////////////
//////////SECTION 1 - constants/////////////////
////////////////////////////////////////////////

//including all external libraries
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

//defining objects to be used within the code
SoftwareSerial BTSerial(0,1); // RX , TX
//SoftwareSerial SlaveSerial(12,13); // RX , TX
LiquidCrystal_I2C lcd(0x27,16,4);

//constants - pin numbers
#define House_pin 3
#define Night_pin 4
#define AC_pin 7
#define Night_in A3
#define Therm_in A2
#define Motion_in A1

//constants - thresholds
#define light_threshold 3.3f
#define motion_threshold 3.2f

//global variables, things that can change, but must be stored
int setTemp = 22;
int Temperature = 21;
int counter = 0;
bool armed = false;
bool intruder = false;
int aud = 0;

///////////////////////////////////////////////
///////////////Section 2 set up////////////////
///////////////////////////////////////////////
void setup() {

//setting up the objects for use later in the code
  Serial.begin(9600);
  //SlaveSerial.begin(9600);
  BTSerial.begin(9600);

//setting pinmode, output or input
  pinMode(House_pin, OUTPUT);
  pinMode(AC_pin, OUTPUT);
  pinMode(Night_pin, OUTPUT);
  pinMode(Night_in, INPUT);
  pinMode(Therm_in, INPUT);
  pinMode(Motion_in, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.noCursor();

}



/////////////////////////////////////////////
////////////////program loop/////////////////
/////////////////////////////////////////////

/////////////////////////////////////////////
//////////section 3 - communication//////////
/////////////////////////////////////////////
void loop() {

//defining local variables for communication
String BT_comm;

///////////////////////
////bluetooth board////
///////////////////////

//loops while data is available from the bluetooth board
  while(BTSerial.available()){
    delay(10);
    BT_comm += ((char)BTSerial.read()); //receives data from blutooth board
  }

////////////////////////////
//bluetooth command actions/
////////////////////////////

  // LED enable
   if(BT_comm == "on"){
    digitalWrite(House_pin, HIGH);
  } 

  // LED disable
  else if(BT_comm == "off"){
    digitalWrite(House_pin,LOW);
  }

  // target temperature command
  else if (BT_comm.substring(0, 4) == "temp") {
    setTemp = BT_comm.substring(4).toInt();
    //Serial.println(targetTemp);
  }

  // Set intruder alert status
  else if (BT_comm.substring(0, 3) == "arm") {
    String s = BT_comm.substring(3);
    armed = (s == "true");
    //Serial.println(armStatus);
  }

  // play songs
  else if (BT_comm == "Song1") {
    //Serial.println("play Song1");
    aud = 1;
  }

  else if (BT_comm == "Song2") {
    // play song2
    aud = 2;
  }

  else if (BT_comm == "Song3") {
    // play song3
    aud = 3;
  }

////////////////////////////////////////////////////////
////////////////section 4 - appliances//////////////////
////////////////////////////////////////////////////////

////////////////////////////
//temperature reading///////
////////////////////////////

      //reads the output of the thermistor and converts it to voltage
      //between 0 and 5. reading input reads as 0 - 1023, so dividing
      //by 204.6 will convert this value to its corresponding voltage
      //reading
  float therm_read = (analogRead(Therm_in) / 204.6);
  
        //minimum output of thermistor is 0.1, if lower, something is wrong
        //makes sure the read value is acceptable
  if(therm_read >= 0.1){

          //converts voltage reading to a temperature
          //equation taken from graph in thermistor datasheet
    Temperature = (int)((therm_read - 0.5) * 100);

  }

  int Farenheight = (int)( ((float)Temperature) / ( 5.0 / 9.0 )  + 32.0);
  
//////////////////////////////
/////////nightlight///////////
//////////////////////////////

      //reads the input value of the photoresistor
      //and converts the input value to a voltage
      //similar to what is done to the thermistor
      //reading
  int night_read = (analogRead(Night_in) / 204.6);

        //determines if read voltage is less than threshold value
        //if it is, it turns the nightlight on
  if(night_read < light_threshold){
    digitalWrite(Night_pin, HIGH); 
  }
  
        //if the read voltage is not less than the threshold value
        //the nightlight is turned off
  else{
    digitalWrite(Night_pin, LOW);
  }



////////////////////////////
///////motion sensor////////
////////////////////////////

        //if motion sensor is armed, read from sensor
  if(armed){

          //reads the output from the motion sensor and converts
          //it to its corresponding voltage level similar to the
          //nightlight and thermistor
    float motion_read = (analogRead(Motion_in) / 204.6);

          //if the motion sensor reading is above threshold
          //sound the alarm
    if(motion_read > motion_threshold){

      //sound alarm
      intruder = true;
      aud = 4;
       
    }
    
  }

////////////////////////////
////////////LCD/////////////
////////////////////////////

  if( (counter % 100) == 0 ){

    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print(Temperature);
    lcd.write(0xDF);
    lcd.print("C  ");
    
    lcd.print(Farenheight);
    lcd.write(0xDF);
    lcd.print("F");
    
    if(armed){

      lcd.setCursor(3,1);
      lcd.print("System: armed");
    
    }
    else{

      lcd.setCursor(2,1);
      lcd.print("System: disarmed");
    
    }

    switch(aud){

      case 1:
        lcd.setCursor(7,3);
        lcd.print("Song 1");
        break;

      case 2:
        lcd.setCursor(7,3);
        lcd.print("Song 2");
        break;

      case 3:
        lcd.setCursor(7,3);
        lcd.print("Song 3");
        break;

      case 4:
        lcd.setCursor(7,3);
        lcd.print("Alarm!");
        break;

      default:
        lcd.setCursor(6,3);
        lcd.print("No Audio");
      
    }

    if(setTemp <= Temperature){
      digitalWrite(AC_pin, HIGH);
    }
    else{
      digitalWrite(AC_pin, LOW);
    }

}

////////////////////////////
//////////sending///////////
////////////////////////////

  counter++;

  if (counter >= 200){
    counter = 0;
  }

  if( counter == 0 ){

    // Send data to phone
    String output = "";

    String temp = String(Temperature);

    String night_s = (night_read < light_threshold) ? "on" : "off";

    String intruderStatus = (intruder) ? "t" : "f"; // t or f

    String armedStatus = (armed) ? "t" : "f"; // t or f

    output = temp + '*' + night_s + '*' + intruderStatus + '*' +  armedStatus + '*';

    Serial.println(output);

    intruder = false;

  }
        //delays the entire program slightly to 
        //make things a little smoother
  delay(10);
  
}

//////////////////////////////////////////////
////////////////end of program////////////////
//////////////////////////////////////////////
