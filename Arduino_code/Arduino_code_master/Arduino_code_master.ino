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
int setTemp = 70;
int oldTemp = 0;
bool armed = false;

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
    BT_comm += BTSerial.read(); //receives data from blutooth board
  }

////////////////////////////
//blutooth command actions//
////////////////////////////

//if received string is "on" turn light on
  if(BT_comm == "on"){
    Serial.println(BT_comm);
    digitalWrite(House_pin, HIGH);
  }
  
//if received string is "off" turn light off
  else if(BT_comm == "off"){
    Serial.println(BT_comm);
    digitalWrite(House_pin,LOW);
  }

  else if(BT_comm == "true"){
    Serial.println(BT_comm);
    armed = true;
  }

  else if(BT_comm == "false"){
    Serial.println(BT_comm);
    armed = false;
  }

  else if( is_number(BT_comm) ){

    Serial.println(BT_comm);
    setTemp = BT_comm.toInt();
    
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
    int Temperature = (int)((therm_read - 0.5) * 100);

          //determines if temperature has changed (whole numbers)
    if(Temperature != oldTemp){

      oldTemp = Temperature; //stores changed temperature value
      //String ret = String(Temperature,0) + "C"; //creates string to send to bluetooth board
      //BTSerial.print(ret); //sends new temperature to bluetooth board
      
    }
    
  }

  if(setTemp >= oldTemp){

    digitalWrite(AC_pin, HIGH);
    
  }
  else{

    digitalWrite(AC_pin, LOW);
    
  }
  
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
    int motion_read = (analogRead(Motion_in) / 204.6);

          //if the motion sensor reading is above thershold
          //sound the alarm
    if(motion_read > 660){

      //sound alarm
       
    }
    
  }

////////////////////////////
////////////LCD/////////////
////////////////////////////

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(oldTemp);
  lcd.print(0xFD);
  if(armed){

    lcd.setCursor(3,1);
    lcd.print("System: armed");
    
  }
  else{

    lcd.setCursor(2,1);
    lcd.print("System: disarmed");
    
  }
  
  lcd.setCursor(6,3);
  lcd.print("No Audio");

        //delays the entire program slightly to 
        //make things a little smoother
  //delay(10);
  
}

bool is_number(String test){
  
    int i; 

    int size = test.length();
    char buffer[size];

    test.toCharArray(buffer, size);
    
    for( i = 0; i < size; i++ ){

      if(!isdigit(buffer[i])){

        return false;
        
      }
      
    }
    
    return true;

}

//////////////////////////////////////////////
////////////////end of program////////////////
//////////////////////////////////////////////
