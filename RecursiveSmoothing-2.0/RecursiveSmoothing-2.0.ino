#include <ResponsiveAnalogRead.h>
#include "FastLED.h"

// How many leds are in the strip?
#define NUM_LEDS 30

// Data pin that led data will be written out over
#define DATA_PIN 3
#define BRIGHTNESS  60
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define MAX_SOUND_LEVEL 740
#define MIN_SOUND_LEVEL 10

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

const int ledPin = 13;
const int sensorPin = A0;

//a value from 0 to 1 that controls the amount of easing.
//Defaults to 0.01. Increase this to lessen the amount of easing (such as 0.1) 
//and make the responsive values more responsive, 
//but doing so may cause more noise to seep through if sleep is not enabled.
const float snapMultiplier  = 0.01;

ResponsiveAnalogRead analog(sensorPin, true, snapMultiplier);

byte colorIndex = 0;

void setup() {
  delay(2000);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS);
  ledStripOff();
  analog.setAnalogResolution(MAX_SOUND_LEVEL);
  delay(1000);
}

void loop() {
  analog.update();
  timer();
}

void ledStripOff(){
  for(int currentLed = 0; currentLed < NUM_LEDS; currentLed++) {
    leds[currentLed] = CRGB::Black;
    FastLED.show();
  }
}

void timer(){
  static const unsigned long refreshInterval = 100;//milliseconds
  static unsigned long lastRefreshTime = 0;

  if (millis() - lastRefreshTime >= refreshInterval){
    lastRefreshTime+=refreshInterval;
    ledColorSetting();
    colorIndex+=10;
  }
}

void ledColorSetting(){  
  int ledToLight = map(analog.getValue(), MIN_SOUND_LEVEL, MAX_SOUND_LEVEL, 0, NUM_LEDS);
  
  fill_rainbow( leds, NUM_LEDS, colorIndex, 45);
  for(int i = ledToLight; i < NUM_LEDS; i++) {
    //if led is already off then leave the loop
    if (leds[i].r == 0 && leds[i].g == 0 && leds[i].b == 0){
      break;
    }
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}
