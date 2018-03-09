#include "FastLED.h"

// How many leds are in the strip?
#define NUM_LEDS 30

// Data pin that led data will be written out over
#define DATA_PIN 3
#define BRIGHTNESS  60
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

const int ledPin = 13;
byte colorIndex = 0;
int lastLedOnCount = 0;

void setup() {
  delay(2000);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS);
  ledStripOff();
  delay(1000);
}

void loop() {
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
    ledColorSetting();
  }
}

void ledColorSetting(){  
  colorIndex+=5;
  fill_rainbow( leds, NUM_LEDS, colorIndex, 45);
  FastLED.show();
}
