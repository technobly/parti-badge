#ifndef PARTICLE_LEDS_H
#define PARTICLE_LEDS_H

#include "Particle.h"

#define CHOICE_OFF 0  //Used to control LEDs
#define CHOICE_NONE 0 //Used to check buttons
#define CHOICE_RED (1 << 0)
#define CHOICE_GREEN (1 << 1)
#define CHOICE_BLUE (1 << 2)
#define CHOICE_YELLOW (1 << 3)

void initLEDButtons();
void toggleAllButtons(int state);
void setLEDs(byte leds);
void toner(byte which, int buzz_length_ms);
void changeLED();
void fadeAllIn();
void fadeAllOut();

void ledChase();
void ledPulse();
void ledPulseChase();
void ledRandom();
void ledSeeSaw();

#endif
