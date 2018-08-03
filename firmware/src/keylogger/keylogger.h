#include "../images/konami.h"

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

void showKonami(Adafruit_SSD1306* display) {
  display->clearDisplay();
  display->display();
  display->setCursor(0, 0);
  display->drawBitmap(0, 0, konamiLogo, 128, 64, 1);
  display->display();
  display->startscrollleft(0x00, 0x0F);
}

void checkKeyProgress(int currentKey, Adafruit_SSD1306* display) {
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
      fireKonamiEvent();
      showKonami(display);
      resetDisplayBools();
      codeEntered = false;
      currentProgress = 0;
      inCodeMode = false;
    }
  }
}
