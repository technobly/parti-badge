#include "parti-badge.h"
#include "Debounce.h"
#include "Adafruit_SSD1306.h"
#include "display/display.h"

extern Adafruit_SSD1306 display;
extern Debounce redButtonADebouncer;
extern Debounce blueButtonBDebouncer;
extern Debounce greenButtonCDebouncer;
extern Debounce yellowButtonDDebouncer;
extern Debounce joystickUpDebouncer;
extern Debounce joystickDownDebouncer;
extern Debounce joystickLeftDebouncer;
extern Debounce joystickRightDebouncer;
extern Debounce joystickCenterDebouncer;
extern bool checkingInputs;

static const unsigned int codeKeys[] = {
  JOYSTICK_UP,
  JOYSTICK_UP,
  JOYSTICK_DOWN,
  JOYSTICK_DOWN,
  JOYSTICK_LEFT,
  JOYSTICK_RIGHT,
  JOYSTICK_LEFT,
  JOYSTICK_RIGHT,
  BLUE_BUTTON_B,
  RED_BUTTON_A,
  JOYSTICK_CENTER
};

bool codeEntered = false;
int currentProgress = 0;
unsigned long lastCheck = millis();
bool inCodeMode = false;

void checkKeyProgress(int currentKey);

void checkInputSequence()
{
  redButtonADebouncer.update();
  if (redButtonADebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(RED_BUTTON_A);
  }

  blueButtonBDebouncer.update();
  if (blueButtonBDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(BLUE_BUTTON_B);
  }

  joystickUpDebouncer.update();
  if (joystickUpDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_UP);
  }

  joystickDownDebouncer.update();
  if (joystickDownDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_DOWN);
  }

  joystickLeftDebouncer.update();
  if (joystickLeftDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_LEFT);
  }

  joystickRightDebouncer.update();
  if (joystickRightDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_RIGHT);
  }

  joystickCenterDebouncer.update();
  if (joystickCenterDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_CENTER);
  }

  checkingInputs = false;
}



void checkKeyProgress(int currentKey) {
  if (lastCheck + 250 <= millis()) {
    lastCheck = millis();

    if (codeKeys[currentProgress] == currentKey) {
      inCodeMode = true;
      if (currentProgress+1 == sizeof(codeKeys)/4) {
        codeEntered = true;
      } else {
        currentProgress++;
      }
    } else {
      currentProgress = 0;
      inCodeMode = false;
    }

    if (codeEntered) {
      showKonami();
      resetDisplayBools();
      codeEntered = false;
      currentProgress = 0;
      inCodeMode = false;
    }
  }
}
