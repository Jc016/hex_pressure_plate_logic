#include "HX711.h"
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      84

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 0; // delay for half a second
bool activationState, prevActivationState,statusChanged;
int threshold = 500;
HX711 scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library
float value;

void setup() {
  
  Serial.begin(38400);
  scale.set_scale(400.f);   // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();	// reset the scale to 0
  statusChanged = false;
  activationState = false;
  prevActivationState = false;
  pixels.begin(); // This initializes the NeoPixel library.
  printHexStateToLeds();
}

void loop() {

  readPressurePlate();
  if(statusChanged){
    printHexStateToLeds();
  }
  
}

void printHexStateToLeds(){
  
  if(activationState){
     Serial.println("on");
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(255,255,255)); // Moderately bright green color.   
    }   
  }else{
    Serial.println("off");
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    } 
  }

  pixels.show();
  
}

void readPressurePlate(){
  
  value=scale.get_units();
  value = abs(value);
  Serial.println("value_read:" + String(value));
  activationState = value >= threshold;
  statusChanged = activationState != prevActivationState;
  prevActivationState= activationState;

}

