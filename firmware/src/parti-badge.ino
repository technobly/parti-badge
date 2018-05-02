#include "Particle.h"

// OLED Includes
#include <SPI.h>
#include <Wire.h>

// OLED Includes
#include "Adafruit_SSD1306.h"

// NeoPixel Includes
#include "neopixel.h"

// Pins, pins, pins
#define LED      D7
#define NEOPIXEL_PIN D2
#define BUTTON_A D3
#define BUTTON_B D4
#define BUTTON_C D5
#define OLED_RESET D0

#define TEXTARRAYSIZE 6

// All in seconds
#define TEXTUPDATEINTERVAL 10

long lastDisplayChange = 0;
int textIndex = 0;
bool updateDisplayRotator = true;
String textStrings[TEXTARRAYSIZE] = { "Brandon", "Satrom", "DevRel @", "Particle", "@Brandon", "Satrom" };

// Loop control booleans
bool displayUpdating = false;
bool rainbowAnimationsPlaying = false;
bool theaterChaseAnimationsPlaying = false;
bool clearingDisplay = false;

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_NeoPixel strip(32, NEOPIXEL_PIN, WS2812B);

void setup() {
  // Subscribe to IFTTT Event when a person tweets the badge
  Particle.subscribe("New_Tweet", notifyNewTweet);

  // Configure Hardware Input Buttons
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // Initialize with the I2C addr 0x3C (for the 128x32)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.display();
  delay(1000);

  // Start the process to paint the OLED
  paintScreen();

  // Init NeoPixel display
  initNeoPixelDisplay();
}

void loop() {
  // If button C is pressed, clear the display
  if (!digitalRead(BUTTON_C) && !clearingDisplay) {
    clearingDisplay = true;
    clearNeoPixelDisplay();
    clearingDisplay = false;

    return;
  }

  if (!digitalRead(BUTTON_B) && !rainbowAnimationsPlaying) {
    rainbowAnimationsPlaying = true;

    clearNeoPixelDisplay();
    strip.setBrightness(15);

    rainbow(20);
    rainbowCycle(20);
    theaterChaseRainbow(50);

    clearNeoPixelDisplay();

    rainbowAnimationsPlaying = false;

    return;
  }

  if (!digitalRead(BUTTON_A) && !theaterChaseAnimationsPlaying) {
    theaterChaseAnimationsPlaying = true;

    clearNeoPixelDisplay();
    strip.setBrightness(15);

    // Send a theater pixel chase in...
    theaterChase(strip.Color(127, 127, 127), 50); // White
    theaterChase(strip.Color(127, 0, 0), 50); // Red
    theaterChase(strip.Color(0, 0, 127), 50); // Blue

    clearNeoPixelDisplay();

    theaterChaseAnimationsPlaying = false;

    return;
  }

  if (millis() - lastDisplayChange > TEXTUPDATEINTERVAL * 1000 && updateDisplayRotator && !displayUpdating) {
    displayUpdating = true;
    lastDisplayChange = millis();

    paintScreen();
    displayUpdating = false;

    return;
  }
}

void initNeoPixelDisplay() {
  strip.begin();
  strip.setBrightness(15);

  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
}

void clearNeoPixelDisplay() {
  strip.setBrightness(0);
  strip.show();
}

void paintScreen() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  scrollText(textStrings[textIndex], textStrings[textIndex + 1], 2);
  if (textIndex + 2 == TEXTARRAYSIZE) {
    textIndex = 0;
  } else {
    textIndex = textIndex + 2;
  }
}

// Set scrolling text on the OLED screen
void scrollText(String line1, String line2, int textSize) {
  display.setTextWrap(false);
  display.setTextSize(textSize);

  display.println(line1);
  display.println(line2);
  display.display();
  display.startscrollleft(0x00, 0x0F);
}

void notifyNewTweet (const char *event, const char *data) {
  clearNeoPixelDisplay();

  strip.setBrightness(15);
  theaterChase(strip.Color(0, 0, 127), 50); // Blue
  theaterChase(strip.Color(255, 255, 255), 50); // White
  theaterChase(strip.Color(0, 0, 127), 50); // Blue

  clearNeoPixelDisplay();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
