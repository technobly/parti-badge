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

void checkKeyProgress(int currentKey) {
  if (lastCheck + 250 <= millis()) {
    lastCheck = millis();

    if (codeKeys[currentProgress] == currentKey) {
      Serial.println("Pressed!");
      inCodeMode = true;
      if (currentProgress+1 == sizeof(codeKeys)/4) {
        codeEntered = true;
      } else {
        currentProgress++;
      }
    } else {
      Serial.println("Reset!");
      currentProgress = 0;
      inCodeMode = false;
    }

    if (codeEntered) {
      Serial.println("Konami!");
      fireKonamiEvent();
      codeEntered = false;
      currentProgress = 0;
      inCodeMode = false;
    }
  }
}
