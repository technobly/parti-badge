/*
 *      Copyright 2018 Particle Industries, Inc.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */
/*
 *      Project: #PartiBadge, 2018 THAT Conference Edition
 *
 *      Description:
 *          This is the firmware for a custom, Photon-based badge PCB that includes:
 *              1. A 1" OLED Screen
 *              2. An SMD Piezo buzzer
 *              3. A SPDT Switch
 *              4. A 5-way joystick
 *              5. An SMD Si7021 temperature and Humidity sensor
 *              6. 4 Tactile LED Buttons in Red, Blue, Green and Yellow/Orange
 *              7. An I2C-Compatible breakout for #BadgeLife add-ons
 *              8. An IR Receiver on the Photon TX pin
 *
 */

#include "Particle.h"
#include "Debounce.h"

// Display includes
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// Menu System include
#include "mainmenu.h"
#include <qMenuSystem.h>

// Interrupts
#include "interrupts/interrupts.h"

// Sub-system includes
#include "display/display.h"
#include "games/games.h"

#include "parti-badge.h" // #define pin assignments and other general macros
#include "music/tones.h" // Peizo Sounds
#include "music/roll.h"
#include "WearerInfo/WearerInfo.h"

#include "Adafruit_Si7021.h"
#include "events/events.h"

SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

PRODUCT_ID(7775);
PRODUCT_VERSION(18);

// Init Display
Adafruit_SSD1306 display(RESET);
void resetDisplayBools();

// Menu Init
qMenuSystem menu = qMenuSystem(&display);

#include "simonsays/simon.h" // Simon Says Code

String deviceId;
WearerInfo wearerInfo;

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

// Initialize Si7021 sensor
Adafruit_Si7021 envSensor = Adafruit_Si7021();
int currentTemp;
int currentHumidity;

// Timing variables
unsigned long elapsedTime;
unsigned long startTime = 0;
unsigned long previousEnvReading = 0;

// Wearer details
String wearerFirstName;
String wearerLastName;
String wearerTwitter;

// Default to display mode, but we'll determine this based on a
// the user holding a button down at startup
int badgeMode = DISPLAY_MODE;

// Display variables
bool displayingTemp = false;
bool displayingLogo = false;
bool displayingTitle = false;
bool displayingWearerDetails = false;
bool displayingAnimations = false;
bool animationsInterrupted = false;
bool displayingEtchASketch = false;
bool playingRoll = false;
bool inCodeMode = false;
bool menuShowing = false;

#include "keylogger/keylogger.h"
#include "animations/animations.h"

// Display state management
bool titleShown = false;
bool buttonsInitialized = false;
bool checkingInputs = false;

volatile byte btncounter = 0;
volatile byte btnid  = 0;
byte appmode = 0;

void setup()
{
  Serial.begin(115200);
  resetDisplayBools();

  // Get the current deviceId
  deviceId = System.deviceID();

  // Initialize Temp and Humidity sensor
  while (!envSensor.begin());

  // Init OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Show Particle Splashscreen
  showSplashscreen();

  // Show the title screen
  showTitle();

  // Set up cloud variables and functions
  cloudInit();

  rollSetup();

  // Set the Piezo buzzer as an output
  pinMode(BUZZER_PIN, OUTPUT);

  // Init the LED and Joystick Buttons
  initButtons();

  // Set up Interrupts
  setupJoystickInterrupts();

  // Get an initial temp and humidity reading
  getTempAndHumidity();

  //Init Tactile LED Buttons
  initLEDButtons();

  // Play a startup sound on the Piezo
  if (!startupSoundPlayed)
    playStartup(BUZZER_PIN);

  // Determine if we're in display or game mode
  checkBadgeMode();

  // Connect to the Particle device cloud
  Particle.connect();

  // Fetch badge wearer details from EEPROM
  initWearerDetails();

  // Scroll the title text on the screen
  display.startscrollleft(0x00, 0x0F);
}

void loop()
{
  unsigned long currentMillis = millis();
  int keycode=0;
  int clickedItem=0;

  if (menuShowing == false) {
    redButtonADebouncer.update();
    blueButtonBDebouncer.update();
    greenButtonCDebouncer.update();
    yellowButtonDDebouncer.update();
    joystickCenterDebouncer.update();

    if (redButtonADebouncer.read() == LOW ||
      blueButtonBDebouncer.read() == LOW ||
      greenButtonCDebouncer.read() == LOW ||
      yellowButtonDDebouncer.read() == LOW ||
      joystickCenterDebouncer.read() == LOW) {
      menuShowing = true;

      clearScreen();
      menu.InitMenu(mnuRoot, cntRoot, 1);
    }
  }

  if (btncounter > 0 && menuShowing)
  {
    switch(btnid)
    {
      case 1:
        clickedItem=menu.ProcessMenu(ACTION_SELECT);
        btncounter--;
        break;
      case 2:
        menu.ProcessMenu(ACTION_DOWN);
        btncounter--;
        break;
      case 3:
        menu.ProcessMenu(ACTION_UP);
        btncounter--;
        break;
      case 4:
        clickedItem=menu.ProcessMenu(ACTION_BACK);
        btncounter--;
        break;
    }
  }

  if (clickedItem > 0 && menuShowing) {
    if (menu.CurrentMenu == mnuRoot) {
      switch (clickedItem)
      {
        case 1:
          menu.InitMenu((const char ** )mnuDisplay, cntDisplay,1);
          break;
        case 2:
          menu.InitMenu((const char ** )mnuMesh, cntMesh,1);
          break;
        case 3:
          menu.InitMenu((const char ** )mnuSensors, cntSensors,1);
          break;
        case 4:
          menu.InitMenu((const char ** )mnuMusic, cntMusic,1);
          break;
        case 5:
          menu.InitMenu((const char ** )mnuGames, cntGames,1);
          break;
        case 6:
          menu.InitMenu((const char ** )mnuAnimations, cntAnimations,1);
          break;
        case 7:
          menu.InitMenu((const char ** )mnuBlinky, cntBlinky,1);
          break;
        case 8:
          displayCredits();
          break;
      }
    }
    // Logic for sub menus
    else if (menu.CurrentMenu == mnuDisplay) {
      switch (clickedItem)
      {
        case 1:
          showTitle();
          display.startscrollleft(0x00, 0x0F);
          break;
        case 2:
          displayWearerDetails();
          break;
        case 3:
          displayTwitterHandle();
          break;
        case 4:
          showTempAndHumidity();
          break;
        case 5:
          menu.InitMenu(mnuRoot, cntRoot, 1);
          break;
      }
    } else if (menu.CurrentMenu == mnuGames) {
      switch (clickedItem) {
        case 1:
          break;
        case 2:
          etchASketch();
          break;
        case 3:
          break;
        case 4:
          menu.InitMenu(mnuRoot, cntRoot, 1);
          break;
      }
    }
  } else if (clickedItem == -1 && menuShowing) {
    if (menu.CurrentMenu == mnuRoot) { /* In root, do nothing */ }

    //2nd level menus
    else if (menu.CurrentMenu == mnuDisplay) {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 1);
    }
    else if (menu.CurrentMenu == mnuGames) {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 5);
    }
  }

  if (badgeMode == DISPLAY_MODE) {
    if (currentMillis - previousEnvReading > TEMP_CHECK_INTERVAL) {
      previousEnvReading = currentMillis;
      getTempAndHumidity();
    }

    if (displayingEtchASketch){
      etchASketch();
    }

    if (digitalRead(D7) == HIGH || playingRoll) {
      playingRoll = true;

      playRoll(&display);
    }

    checkInputSequence();
  }
  else if (badgeMode == GAME_MODE) {
    configureGame();

    playGame();
  }
}

// Switch to Simon Says mode if the A and B buttons are held down at startup
void checkBadgeMode()
{
  redButtonADebouncer.update();
  blueButtonBDebouncer.update();

  if (redButtonADebouncer.read() == LOW && blueButtonBDebouncer.read() == LOW)
  {
    badgeMode = GAME_MODE;
  }
  else
  {
    badgeMode = DISPLAY_MODE;
  }
}

// Init our Device Cloud variables and functions
void cloudInit()
{
  Particle.variable("wearerFName", wearerFirstName);
  Particle.variable("wearerLName", wearerLastName);
  Particle.variable("wearerTwitter", wearerTwitter);

  Particle.variable("currentTemp", currentTemp);
  Particle.variable("currentHu", currentHumidity);

  Particle.function("updateFName", updateFirstNameHandler);
  Particle.function("updateLName", updateLastNameHandler);
  Particle.function("updateTwitter", updateTwitterHandler);

  Particle.function("checkTemp", checkTempHandler);
}

// Fetch wearer details from our WearerInfo class
void initWearerDetails()
{
  wearerInfo = WearerInfo();

  if (wearerInfo.isSet())
  {
    wearerFirstName = wearerInfo.getFirstName();
    wearerLastName = wearerInfo.getLastName();
    wearerTwitter = wearerInfo.getTwitter();
  }
}

// Init debouncers for all of our inputs
void initButtons()
{
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

// Set up the tactile LED buttons
void initLEDButtons()
{
  buttonsInitialized = true;

  int del = 300;
  int medDel = 500;

  // Init D7
  pinMode(D7, INPUT_PULLDOWN);

  // Init LEDs and Outputs
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  digitalWrite(RED_LED, HIGH);
  delay(del);
  digitalWrite(BLUE_LED, HIGH);
  delay(del);
  digitalWrite(GREEN_LED, HIGH);
  delay(del);
  digitalWrite(YELLOW_LED, HIGH);
  delay(del);

  toggleAllButtons(LOW);
  delay(medDel);

  toggleAllButtons(HIGH);
  delay(medDel);

  toggleAllButtons(LOW);
  delay(medDel);

  toggleAllButtons(HIGH);
}

// Toggle all the buttons on or off
void toggleAllButtons(int state)
{
  digitalWrite(RED_LED, state);
  digitalWrite(BLUE_LED, state);
  digitalWrite(GREEN_LED, state);
  digitalWrite(YELLOW_LED, state);
}

// Reset all display-related state booleans
void resetDisplayBools()
{
  displayingTemp = false;
  displayingWearerDetails = false;
  displayingLogo = false;
  displayingTitle = false;
  displayingAnimations = false;
  animationsInterrupted = false;
  displayingEtchASketch = false;
  playingRoll = false;
}

// Get temp and humidity from the sensors
void getTempAndHumidity()
{
  int prevTemp = currentTemp;
  int prevHumidity = currentHumidity;

  currentTemp = round((envSensor.readTemperature() * 1.8 + 32.00) * 10) / 10;
  currentHumidity = round(envSensor.readHumidity() * 10) / 10;

  // If either has changed and these values are being displayed, update the display
  if (displayingTemp && (prevTemp != currentTemp || prevHumidity != currentHumidity))
  {
    showTempAndHumidity();
  }

  fireEnvSensorsEvent(currentTemp, currentHumidity);
}

// This shall remain undocumented, for it is a secret... what it do?
void checkInputSequence()
{
  redButtonADebouncer.update();
  if (redButtonADebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(RED_BUTTON_A, &display);
  }

  blueButtonBDebouncer.update();
  if (blueButtonBDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(BLUE_BUTTON_B, &display);
  }

  joystickUpDebouncer.update();
  if (joystickUpDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_UP, &display);
  }

  joystickDownDebouncer.update();
  if (joystickDownDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_DOWN, &display);
  }

  joystickLeftDebouncer.update();
  if (joystickLeftDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_LEFT, &display);
  }

  joystickRightDebouncer.update();
  if (joystickRightDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_RIGHT, &display);
  }

  joystickCenterDebouncer.update();
  if (joystickCenterDebouncer.read() == LOW && !checkingInputs)
  {
    checkingInputs = true;
    checkKeyProgress(JOYSTICK_CENTER, &display);
  }

  checkingInputs = false;
}

//Update the first name when called from a cloud function
int updateFirstNameHandler(String data)
{
  wearerFirstName = data;
  wearerInfo.setFirstName(wearerFirstName);

  fireNamedEvent();
  if (displayingWearerDetails)
  {
    displayWearerDetails();
  }
  return 1;
}

//Update the last name when called from a cloud function
int updateLastNameHandler(String data)
{
  wearerLastName = data;
  wearerInfo.setLastName(wearerLastName);

  fireNamedEvent();
  if (displayingWearerDetails)
  {
    displayWearerDetails();
  }
  return 1;
}

int updateTwitterHandler(String data) {
  wearerTwitter = data;
  wearerInfo.setTwitter(wearerTwitter);

  fireNamedEvent();
  if (displayWearerDetails) {
    displayWearerDetails();
  }
  return 1;
}

// Check the temp and humidity when called from a cloud function
int checkTempHandler(String data)
{
  getTempAndHumidity();

  return 1;
}
