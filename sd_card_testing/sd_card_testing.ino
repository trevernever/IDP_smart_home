/*
 * Play wave audio files with Arduino.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 */
 
#include <SPI.h>     // include Arduino SPI library
#include <SD.h>      // include Arduino SD library
#include "TMRpcm.h"  // include TMRpcm library
 
#define next     2
#define _pause   3
 
TMRpcm audio;
 
File root;
 
void setup(void) {
  Serial.begin(9600);
  pinMode(next,   INPUT_PULLUP);
  pinMode(_pause, INPUT_PULLUP);
 
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("failed!");
    while(true);  // stay here.
  }
  Serial.println("OK!");
 
  audio.speakerPin = 9;  // set speaker output to pin 9
 
  root = SD.open("/");      // open SD card main root
  printDirectory(root, 0);  // print all files names and sizes
 
  audio.setVolume(7);    //   0 to 7. Set volume level
  audio.quality(0);      //  Set 1 for 2x oversampling Set 0 for normal
 
}
 
// main loop
void loop() {
 
  if ( !audio.isPlaying() ) {
    // no audio file is playing
    File entry =  root.openNextFile();  // open next file
    if (! entry) {
      // no more files
      root.rewindDirectory();  // go to start of the folder
      return;
    }
 
    uint8_t nameSize = String(entry.name()).length();  // get file name size
    String str1 = String(entry.name()).substring( nameSize - 4 );  // save the last 4 characters (file extension)
 
    if ( str1.equalsIgnoreCase(".wav") ) {
      // the opened file has '.wav' extension
      audio.play( entry.name() );      // play the audio file
      Serial.print("Playing file: ");
      Serial.println( entry.name() );
    }
 
    else {
      // not '.wav' format file
      entry.close();
      return;
    }
 
    while( debounce(next) ) ;  // wait until 'next' button is released
  }
 
  if ( !digitalRead(next) ) {
    // 'next' button is pressed
    audio.stopPlayback();  // stop playing
    return;
  }
 
  if ( !digitalRead(_pause) ) {
    // '_pause' button is pressed
    audio.pause();      // pauses/unpauses playback
    while( debounce(_pause) ) ;  // wait until '_pause' button is released
  }
 
}
 
// a small function for buttons debounce
bool debounce (int bt)
{
  byte count = 0;
  for(byte i = 0; i < 5; i++)
  {
    if ( !digitalRead(bt) )
      count++;
    delay(10);
  }
 
  if(count > 2)  return 1;
  else           return 0;
}
 
void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
 
// end of code
