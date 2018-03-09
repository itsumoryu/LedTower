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

const int numReadings = 20;
int readings[numReadings]; 
int readIndex = 0;
int total = 0;
int average = 0;

byte colorIndex = 0;
int lastLedOnCount = 0;

void setup() {
  delay(2000);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS);
  ledStripOff();
  for (int i = 0; i < numReadings; i++){
    readings[i] = 0;
  }
  delay(1000);
}

void loop() {
  soundSensorSmoothing();
  timer();
}

void ledStripOff(){
  for(int currentLed = 0; currentLed < NUM_LEDS; currentLed++) {
    leds[currentLed] = CRGB::Black;
    FastLED.show();
  }
}

void timer(){
  static const unsigned long refreshInterval = 50;//milliseconds
  static unsigned long lastRefreshTime = 0;
  
  if (millis() - lastRefreshTime >= refreshInterval){
    lastRefreshTime+=refreshInterval;
//    soundSensorSmoothing();
    ledColorSetting();
  }
}

void soundSensorSmoothing(){
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(sensorPin);
  total = total + readings[readIndex];
  readIndex++;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;
}

void ledColorSetting(){  
  int ledToLight = NUM_LEDS * ((double)average/(double)(MAX_SOUND_LEVEL - MIN_SOUND_LEVEL));
  int diff = lastLedOnCount - ledToLight;
  if (diff > 5 & diff < 10){
    ledToLight = lastLedOnCount - 5;
  }
  else if (diff < -5 && diff > -10){
    ledToLight = lastLedOnCount + 5;
  }
  lastLedOnCount = ledToLight;
  
  colorIndex+=5;
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
