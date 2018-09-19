#ifndef PARTICLE_INPUTS_H
#define PARTICLE_INPUTS_H

#include "Debounce.h"

// Button Debounce Support
extern Debounce redButtonADebouncer;
extern Debounce blueButtonBDebouncer;
extern Debounce greenButtonCDebouncer;
extern Debounce yellowButtonDDebouncer;

// Debouncers for 5-way tactile switch
extern Debounce joystickUpDebouncer;
extern Debounce joystickDownDebouncer;
extern Debounce joystickLeftDebouncer;
extern Debounce joystickRightDebouncer;
extern Debounce joystickCenterDebouncer;

void initInputDebouncers();

#endif
