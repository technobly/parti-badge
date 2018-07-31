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

bool checkingKey = false;
bool codeEntered = false;
int currentProgress = 0;

void checkKeyProgress(int currentKey) {
  if (checkingKey) {
    return;
  }

  checkingKey = true;

  if (codeKeys[currentProgress] == currentKey) {
    Serial.println("Pressed!");
    if (currentProgress+1 == sizeof(codeKeys)) {
      codeEntered = true;
    } else {
      currentProgress++;
    }
  } else {
    Serial.println("Reset!");
    currentProgress = 0;
  }

  if (codeEntered) {
    fireKonamiEvent();
    codeEntered = false;
    currentProgress = 0;
  }

  checkingKey = false;
}
