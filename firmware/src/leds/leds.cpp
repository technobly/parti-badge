#include "Particle.h"
#include "macros.h"
#include "leds.h"
#include "interrupts/interrupts.h"
#include "music/music.h"
#include "display/display.h"

extern byte appmode;
extern byte btncounter;
extern byte btnid;

int LEDnumber = 0;
int fadeIncrement = 5;

void fadeIn(int ledPin, int delayInt)
{
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += fadeIncrement)
  {
    analogWrite(ledPin, fadeValue);
    delay(delayInt);
  }
}

void fadeOut(int ledPin, int delayInt)
{
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= fadeIncrement)
  {
    analogWrite(ledPin, fadeValue);
    delay(delayInt);
  }
}

void fadeAllIn()
{
  for (int fadeValue = 5; fadeValue <= 255; fadeValue += fadeIncrement)
  {
    analogWrite(RED_LED, fadeValue);
    analogWrite(BLUE_LED, fadeValue);
    analogWrite(GREEN_LED, fadeValue);
    analogWrite(YELLOW_LED, fadeValue);
    delay(15);
  }
}

void fadeAllOut()
{
  for (int fadeValue = 255; fadeValue > 0; fadeValue -= fadeIncrement)
  {
    analogWrite(RED_LED, fadeValue);
    analogWrite(BLUE_LED, fadeValue);
    analogWrite(GREEN_LED, fadeValue);
    analogWrite(YELLOW_LED, fadeValue);
    delay(15);
  }
}

// Set up the tactile LED buttons
void initLEDButtons()
{
  int del = 300;
  int medDel = 500;

  // Init D7
  pinMode(D7, INPUT_PULLDOWN);

  // Init LEDs as Outputs
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  fadeAllIn();
  fadeAllOut();
  fadeAllIn();
  fadeAllOut();
  fadeAllIn();

  toggleAllButtons(LOW);
  delay(medDel);

  analogWrite(RED_LED, 255);
  delay(del);
  analogWrite(BLUE_LED, 255);
  delay(del);
  analogWrite(GREEN_LED, 255);
  delay(del);
  analogWrite(YELLOW_LED, 255);
}

// Toggle all the buttons on or off
void toggleAllButtons(int state)
{
  int analogState = state == HIGH ? 255 : 0;

  analogWrite(RED_LED, analogState);
  analogWrite(BLUE_LED, analogState);
  analogWrite(GREEN_LED, analogState);
  analogWrite(YELLOW_LED, analogState);
}

// Lights a given LEDs
// Pass in a byte that is made up from CHOICE_RED, CHOICE_YELLOW, etc
void setLEDs(byte leds)
{
  if ((leds & CHOICE_RED) != 0)
    analogWrite(RED_LED, 255);
  else
    analogWrite(RED_LED, 0);

  if ((leds & CHOICE_GREEN) != 0)
    analogWrite(GREEN_LED, 255);
  else
    analogWrite(GREEN_LED, 0);

  if ((leds & CHOICE_BLUE) != 0)
    analogWrite(BLUE_LED, 255);
  else
    analogWrite(BLUE_LED, 0);

  if ((leds & CHOICE_YELLOW) != 0)
    analogWrite(YELLOW_LED, 255);
  else
    analogWrite(YELLOW_LED, 0);
}

// Light an LED and play tone
// Red, upper left:     440Hz - 2.272ms - 1.136ms pulse
// Green, upper right:  880Hz - 1.136ms - 0.568ms pulse
// Blue, lower left:    587.33Hz - 1.702ms - 0.851ms pulse
// Yellow, lower right: 784Hz - 1.276ms - 0.638ms pulse
void toner(byte which, int buzz_length_ms)
{
  setLEDs(which); //Turn on a given LED

  //Play the sound associated with the given LED
  switch (which)
  {
  case CHOICE_RED:
    buzz_sound(buzz_length_ms, 1136);
    break;
  case CHOICE_GREEN:
    buzz_sound(buzz_length_ms, 568);
    break;
  case CHOICE_BLUE:
    buzz_sound(buzz_length_ms, 851);
    break;
  case CHOICE_YELLOW:
    buzz_sound(buzz_length_ms, 638);
    break;
  }

  setLEDs(CHOICE_OFF); // Turn off all LEDs
}

// Each time this function is called the board moves to the next LED
void changeLED()
{
  setLEDs(1 << LEDnumber); // Change the LED

  LEDnumber++; // Goto the next LED
  if (LEDnumber > 3)
    LEDnumber = 0; // Wrap the counter if needed
}

void ledChase()
{
  appmode = 1;
  btnid = 0;
  int del = 100;

  setupBackButtonInterrupt();
  toggleAllButtons(LOW);

  while (appmode)
  {
    displayCarousel();

    analogWrite(BLUE_LED, 255);
    delay(del);
    analogWrite(GREEN_LED, 255);
    delay(del);
    analogWrite(YELLOW_LED, 255);
    delay(del);
    analogWrite(RED_LED, 255);
    delay(del);

    analogWrite(BLUE_LED, 0);
    delay(del);
    analogWrite(GREEN_LED, 0);
    delay(del);
    analogWrite(YELLOW_LED, 0);
    delay(del);
    analogWrite(RED_LED, 0);
    delay(del);
  }
}

void ledPulseChase()
{
  appmode = 1;
  btnid = 0;
  int del = 5;
  int fadeDel = 10;

  setupBackButtonInterrupt();
  toggleAllButtons(LOW);

  while (appmode)
  {
    displayCarousel();

    fadeIn(BLUE_LED, fadeDel);
    delay(del);
    fadeIn(GREEN_LED, fadeDel);
    delay(del);
    fadeIn(YELLOW_LED, fadeDel);
    delay(del);
    fadeIn(RED_LED, fadeDel);
    delay(del);

    fadeOut(BLUE_LED, fadeDel);
    delay(del);
    fadeOut(GREEN_LED, fadeDel);
    delay(del);
    fadeOut(YELLOW_LED, fadeDel);
    delay(del);
    fadeOut(RED_LED, fadeDel);
    delay(del);
  }
}

void ledPulse()
{
  appmode = 1;
  btnid = 0;

  setupBackButtonInterrupt();
  toggleAllButtons(LOW);

  while (appmode)
  {
    displayCarousel();

    fadeAllIn();
    fadeAllOut();
  }
}

void ledRandom()
{
  appmode = 1;
  btnid = 0;
  int r = 0;
  int del = 150;

  pin_t leds[4] = {RED_LED, BLUE_LED, GREEN_LED, YELLOW_LED};

  setupBackButtonInterrupt();
  toggleAllButtons(LOW);

  while (appmode)
  {
    displayCarousel();

    r = random(4);

    analogWrite(leds[r], 255);
    delay(del);
    analogWrite(leds[r], 0);
  }
}

void ledSeeSaw()
{
  appmode = 1;
  btnid = 0;
  int del = 150;

  setupBackButtonInterrupt();
  toggleAllButtons(LOW);

  while (appmode)
  {
    displayCarousel();

    analogWrite(BLUE_LED, 255);
    delay(del);
    analogWrite(GREEN_LED, 255);
    delay(del);
    analogWrite(YELLOW_LED, 255);
    delay(del);
    analogWrite(RED_LED, 255);
    delay(del);

    analogWrite(RED_LED, 0);
    delay(del);
    analogWrite(YELLOW_LED, 0);
    delay(del);
    analogWrite(GREEN_LED, 0);
    delay(del);
    analogWrite(BLUE_LED, 0);
    delay(del);
  }
}
