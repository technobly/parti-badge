#include "macros.h"
#include "leds/leds.h"
#include "inputs.h"

// Button Debounce Support
Debounce redButtonADebouncer = Debounce();
Debounce blueButtonBDebouncer = Debounce();
Debounce greenButtonCDebouncer = Debounce();
Debounce yellowButtonDDebouncer = Debounce();

// Debouncers for 5-way tactile switch
Debounce joystickUpDebouncer = Debounce();
Debounce joystickDownDebouncer = Debounce();
Debounce joystickLeftDebouncer = Debounce();
Debounce joystickRightDebouncer = Debounce();
Debounce joystickCenterDebouncer = Debounce();

// Init debouncers for all of our inputs
void initInputDebouncers()
{
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);

  // Init Buttons as Inputs
  redButtonADebouncer.attach(RED_BUTTON_A, INPUT_PULLUP);
  redButtonADebouncer.interval(DEBOUNCE_INTERVAL);
  blueButtonBDebouncer.attach(BLUE_BUTTON_B, INPUT_PULLUP);
  blueButtonBDebouncer.interval(DEBOUNCE_INTERVAL);
  greenButtonCDebouncer.attach(GREEN_BUTTON_C, INPUT_PULLUP);
  greenButtonCDebouncer.interval(DEBOUNCE_INTERVAL);
  yellowButtonDDebouncer.attach(YELLOW_BUTTON_D, INPUT_PULLUP);
  yellowButtonDDebouncer.interval(DEBOUNCE_INTERVAL);

  // Joystick buttons as Inputs
  joystickUpDebouncer.attach(JOYSTICK_UP, INPUT_PULLUP);
  joystickUpDebouncer.interval(DEBOUNCE_INTERVAL);
  joystickDownDebouncer.attach(JOYSTICK_DOWN, INPUT_PULLUP);
  joystickDownDebouncer.interval(DEBOUNCE_INTERVAL);
  joystickLeftDebouncer.attach(JOYSTICK_LEFT, INPUT_PULLUP);
  joystickLeftDebouncer.interval(DEBOUNCE_INTERVAL);
  joystickRightDebouncer.attach(JOYSTICK_RIGHT, INPUT_PULLUP);
  joystickRightDebouncer.interval(DEBOUNCE_INTERVAL);
  joystickCenterDebouncer.attach(JOYSTICK_CENTER, INPUT_PULLUP);
  joystickCenterDebouncer.interval(DEBOUNCE_INTERVAL);
}

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
byte checkButton()
{
  redButtonADebouncer.update();
  blueButtonBDebouncer.update();
  greenButtonCDebouncer.update();
  yellowButtonDDebouncer.update();

  if (redButtonADebouncer.read() == LOW)
    return (CHOICE_RED);
  else if (greenButtonCDebouncer.read() == LOW)
    return (CHOICE_GREEN);
  else if (blueButtonBDebouncer.read() == LOW)
    return (CHOICE_BLUE);
  else if (yellowButtonDDebouncer.read() == LOW)
    return (CHOICE_YELLOW);

  return (CHOICE_NONE); // If no button is pressed, return none
}
