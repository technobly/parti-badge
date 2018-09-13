#include "Particle.h"
#include "parti-badge.h"
#include "leds.h"
#include "music/music.h"

int LEDnumber = 0;

// Toggle all the buttons on or off
void toggleAllButtons(int state)
{
  digitalWrite(RED_LED, state);
  digitalWrite(BLUE_LED, state);
  digitalWrite(GREEN_LED, state);
  digitalWrite(YELLOW_LED, state);
}

// Lights a given LEDs
// Pass in a byte that is made up from CHOICE_RED, CHOICE_YELLOW, etc
void setLEDs(byte leds)
{
  if ((leds & CHOICE_RED) != 0)
    digitalWrite(RED_LED, HIGH);
  else
    digitalWrite(RED_LED, LOW);

  if ((leds & CHOICE_GREEN) != 0)
    digitalWrite(GREEN_LED, HIGH);
  else
    digitalWrite(GREEN_LED, LOW);

  if ((leds & CHOICE_BLUE) != 0)
    digitalWrite(BLUE_LED, HIGH);
  else
    digitalWrite(BLUE_LED, LOW);

  if ((leds & CHOICE_YELLOW) != 0)
    digitalWrite(YELLOW_LED, HIGH);
  else
    digitalWrite(YELLOW_LED, LOW);
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
  switch(which)
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

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
byte checkButton()
{
  if (digitalRead(RED_BUTTON_A) == 0) return(CHOICE_RED);
  else if (digitalRead(GREEN_BUTTON_C) == 0) return(CHOICE_GREEN);
  else if (digitalRead(BLUE_BUTTON_B) == 0) return(CHOICE_BLUE);
  else if (digitalRead(YELLOW_BUTTON_D) == 0) return(CHOICE_YELLOW);

  return(CHOICE_NONE); // If no button is pressed, return none
}

// Each time this function is called the board moves to the next LED
void changeLED()
{
  setLEDs(1 << LEDnumber); // Change the LED

  LEDnumber++; // Goto the next LED
  if(LEDnumber > 3) LEDnumber = 0; // Wrap the counter if needed
}
