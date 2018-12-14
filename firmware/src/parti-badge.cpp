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

// Display includes
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// Menu System include
#include "mainmenu.h"
#include "qMenuSystem.h"

#include "macros.h" // #define pin assignments and other general macros

// Interrupts
#include "interrupts/interrupts.h"

// Sub-system includes
#include "inputs/inputs.h"
#include "display/display.h"
#include "games/games.h"
#include "games/simon.h"
#include "leds/leds.h"
#include "music/music.h"
#include "keylogger/keylogger.h"
#include "sensors/sensors.h"
#include "animations/animations.h"
#include "mesh/mesh.h"
#include "udp/udp.h"

#include "music/roll.h"
#include "WearerInfo/WearerInfo.h"
#include "music/xmassongs.h"

#include "Adafruit_Si7021.h"
#include "events/events.h"

// SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// Init Display
Adafruit_SSD1306 display(RESET);
void resetDisplayBools();

// Menu Init
qMenuSystem menu = qMenuSystem(&display);

String deviceId;
WearerInfo wearerInfo;

// Initialize Si7021 sensor
Adafruit_Si7021 envSensor = Adafruit_Si7021();
int currentTemp;
int currentHumidity;
int currentBatteryReading;

// Timing variables
unsigned long elapsedTime;
unsigned long startTime = 0;
unsigned long previousEnvReading = 0;
unsigned long previousBatteryReading = 0;

// Wearer details
String wearerFirstName;
String wearerLastName;
String wearerTwitter;

// Display variables
extern bool displayingTemp;
extern bool displayingWearerDetails;
extern bool displayingCarousel;
extern bool displayingBattery;
extern bool showingLightAnimation;
bool menuShowing = false;

// Display state management
bool checkingInputs = false;

volatile byte btncounter = 0;
volatile byte btnid = 0;
byte appmode = 0;

int updateFirstNameHandler(String data);
int updateLastNameHandler(String data);
int updateTwitterHandler(String data);
int checkTempHandler(String data);
void cloudInit();
void initWearerDetails();

void setup()
{
  Serial.begin();

  // setup Mesh
  meshSetup();

  resetDisplayBools();

  // Get the current deviceId
  deviceId = System.deviceID();

  // Initialize Temp and Humidity sensor
  while (!envSensor.begin())
    ;

  // Init OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Show Particle Splashscreen
  showSplashscreen();

  // Show the title screen
  showTitle();

  // Set up cloud variables and functions
  cloudInit();

  // Set up mesh subscribe handlers
  setupMeshEvents();

  // Set the Piezo buzzer as an output
  pinMode(BUZZER_PIN, OUTPUT);

  // Init the LED and Joystick Buttons
  initInputDebouncers();

  // Set up Interrupts
  setupJoystickInterrupts();

  // Get an initial temp and humidity reading
  getTempAndHumidity();

  // Get an Initial Battery Reading
  getBatteryReading();

  //Init Tactile LED Buttons
  initLEDButtons();

  // Play a startup sound on the Piezo
  playStartup(BUZZER_PIN, false);

  // Connect to the Particle device cloud
  // Particle.connect();

  // Fetch badge wearer details from EEPROM
  initWearerDetails();

  // Scroll the title text on the screen
  display.startscrollleft(0x00, 0x0F);
}

void loop()
{
  unsigned long currentMillis = millis();
  int clickedItem = 0;

  if (menuShowing == false)
  {
    redButtonADebouncer.update();
    blueButtonBDebouncer.update();
    greenButtonCDebouncer.update();
    yellowButtonDDebouncer.update();
    joystickCenterDebouncer.update();

    if (redButtonADebouncer.read() == LOW ||
        blueButtonBDebouncer.read() == LOW ||
        greenButtonCDebouncer.read() == LOW ||
        yellowButtonDDebouncer.read() == LOW ||
        joystickCenterDebouncer.read() == LOW)
    {
      menuShowing = true;

      clearScreen();
      menu.InitMenu(mnuRoot, cntRoot, 1);
    }
  }

  if (btncounter > 0 && menuShowing)
  {
    switch (btnid)
    {
    case 1:
    case 5:
      clickedItem = menu.ProcessMenu(ACTION_SELECT);
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
      clickedItem = menu.ProcessMenu(ACTION_BACK);
      btncounter--;
      break;
    }
  }

  if (clickedItem > 0 && menuShowing)
  {
    clearScreen();

    if (menu.CurrentMenu == mnuRoot)
    {
      switch (clickedItem)
      {
      case 1:
        menu.InitMenu((const char **)mnuDisplay, cntDisplay, 1);
        break;
      case 2:
        menu.InitMenu((const char **)mnuMesh, cntMesh, 1);
        break;
      case 3:
        menu.InitMenu((const char **)mnuSensors, cntSensors, 1);
        break;
      case 4:
        menu.InitMenu((const char **)mnuMusic, cntMusic, 1);
        break;
      case 5:
        menu.InitMenu((const char **)mnuGames, cntGames, 1);
        break;
      case 6:
        menu.InitMenu((const char **)mnuGraphics, cntGraphics, 1);
        break;
      case 7:
        menu.InitMenu((const char **)mnuAnimations, cntAnimations, 1);
        break;
      case 8:
        menu.InitMenu((const char **)mnuBlinky, cntBlinky, 1);
        break;
      case 9:
        displayCredits();
        break;
      }
    }
    // Logic for sub menus
    else if (menu.CurrentMenu == mnuDisplay)
    {
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
        displayCarousel();
        break;
      case 6:
        menu.InitMenu(mnuRoot, cntRoot, 1);
        break;
      }
    }
    else if (menu.CurrentMenu == mnuMesh)
    {
      switch (clickedItem)
      {
      case 1:
        getMeshNetworkInfo();
        break;
      case 2:
        fireHelloPingEvent();
        break;
      case 3:
        fireGatewayPingEvent();
        break;
      case 4:
        fireStartSpookyEvent();
        break;
      case 5:
        fireStopSpookyEvent();
        break;
      case 6:
        menu.InitMenu(mnuRoot, cntRoot, 3);
        break;
      }
    }
    else if (menu.CurrentMenu == mnuSensors)
    {
      switch (clickedItem)
      {
      case 1:
        showTempAndHumidity();
        break;
      case 2:
        showBattery();
        break;
      case 3:
        menu.InitMenu(mnuRoot, cntRoot, 3);
        break;
      }
    }
    else if (menu.CurrentMenu == mnuMusic)
    {
      switch (clickedItem)
      {
      case 1:
        playStartup(BUZZER_PIN, true);
        break;
      case 2:
        playGameOver(BUZZER_PIN, true);
        break;
      case 3:
        playRoll();
        break;
      case 4:
        playBeegees();
        break;
      case 5:
        playJoyToTheWorld();
        break;
      case 6:
        playWeWishYouAMerryXmas();
        break;
      case 7:
        playRudolfTheRedNosedReindeer();
        break;
      case 8:
        playJingleBells();
        break;
      case 9:
        playSilentNight();
        break;
      case 10:
        menu.InitMenu(mnuRoot, cntRoot, 4);
        break;
      }
    }
    else if (menu.CurrentMenu == mnuGames)
    {
      switch (clickedItem)
      {
      case 1:
        etchASketch();
        break;
      case 2:
        snakeGame();
        break;
      case 3:
        initSimon();
        break;
      case 4:
        menu.InitMenu(mnuRoot, cntRoot, 5);
        break;
      }
    }
    else if (menu.CurrentMenu == mnuGraphics)
    {
      switch (clickedItem)
      {
      case 1:
        showSpark();
        break;
      case 2:
        showSpectra();
        break;
      case 3:
        showMesh();
        break;
      case 4:
        showKonami();
        break;
      case 5:
        menu.InitMenu(mnuRoot, cntRoot, 6);
        break;
      }
    }
    else if (menu.CurrentMenu == mnuAnimations)
    {
      switch (clickedItem)
      {
      case 1:
        snowflakes();
        break;
      case 2:
        drawCircles();
        break;
      case 3:
        drawRectangles();
        break;
      case 4:
        drawTriangles();
        break;
      case 5:
        drawRoundedRectangles();
        break;
      case 6:
        scrollText();
        break;
      case 7:
        cycleAnimations();
        break;
      case 8:
        menu.InitMenu(mnuRoot, cntRoot, 7);
        break;
      }
    }
    else if (menu.CurrentMenu == mnuBlinky)
    {
      switch (clickedItem)
      {
      case 1:
        ledChase();
        break;
      case 2:
        ledPulse(true);
        break;
      case 3:
        ledPulseChase();
        break;
      case 4:
        ledRandom();
        break;
      case 5:
        ledSeeSaw();
        break;
      case 6:
        menu.InitMenu(mnuRoot, cntRoot, 8);
        break;
      }
    }
  }
  else if (clickedItem == -1 && menuShowing)
  {
    if (menu.CurrentMenu == mnuRoot)
    { /* In root, do nothing */
    }

    //2nd level menus
    else if (menu.CurrentMenu == mnuDisplay)
    {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 1);
    }
    else if (menu.CurrentMenu == mnuMesh)
    {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 2);
    }
    else if (menu.CurrentMenu == mnuSensors)
    {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 3);
    }
    else if (menu.CurrentMenu == mnuMusic)
    {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 4);
    }
    else if (menu.CurrentMenu == mnuGames)
    {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 5);
    }
    else if (menu.CurrentMenu == mnuGraphics)
    {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 6);
    }
    else if (menu.CurrentMenu == mnuAnimations)
    {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 7);
    }
    else if (menu.CurrentMenu == mnuBlinky)
    {
      menu.InitMenu((const char **)mnuRoot, cntRoot, 8);
    }
  }

  if (displayingCarousel)
  {
    displayCarousel();
  }

  if (currentMillis - previousBatteryReading > BATT_CHECK_INTERVAL)
  {
    previousBatteryReading = currentMillis;
    getBatteryReading();
  }

  if (currentMillis - previousEnvReading > TEMP_CHECK_INTERVAL)
  {
    previousEnvReading = currentMillis;
    getTempAndHumidity();
  }

  blueButtonBDebouncer.update();
  if (blueButtonBDebouncer.read() == LOW)
  {
    fireTowerSendEvent();
  }

  if (!showingLightAnimation)
  {
    redButtonADebouncer.update();
    if (redButtonADebouncer.read() == LOW)
    {
      analogWrite(RED_LED, 255);
    }
    else
    {
      analogWrite(RED_LED, 0);
    }

    blueButtonBDebouncer.update();
    if (blueButtonBDebouncer.read() == LOW)
    {
      analogWrite(BLUE_LED, 255);
    }
    else
    {
      analogWrite(BLUE_LED, 0);
    }

    greenButtonCDebouncer.update();
    if (greenButtonCDebouncer.read() == LOW)
    {
      analogWrite(GREEN_LED, 255);
    }
    else
    {
      analogWrite(GREEN_LED, 0);
    }

    yellowButtonDDebouncer.update();
    if (yellowButtonDDebouncer.read() == LOW)
    {
      analogWrite(YELLOW_LED, 255);
    }
    else
    {
      analogWrite(YELLOW_LED, 0);
    }
  }
}

// Init our Device Cloud variables and functions
void cloudInit()
{
  Particle.variable("wearerFName", wearerFirstName);
  Particle.variable("wearerLName", wearerLastName);
  Particle.variable("wearerTwttr", wearerTwitter);

  Particle.variable("currentTemp", currentTemp);
  Particle.variable("currentHu", currentHumidity);

  Particle.function("updateFName", updateFirstNameHandler);
  Particle.function("updateLName", updateLastNameHandler);
  Particle.function("updateTwttr", updateTwitterHandler);

  Particle.function("checkTemp", checkTempHandler);

  Particle.subscribe("pulseLed", pulseLEDHandler, MY_DEVICES);
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

//Update the first name when called from a cloud function
int updateFirstNameHandler(String data)
{
  wearerFirstName = data;
  wearerInfo.setFirstName(wearerFirstName);

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

  if (displayingWearerDetails)
  {
    displayWearerDetails();
  }
  return 1;
}

int updateTwitterHandler(String data)
{
  wearerTwitter = data;
  wearerInfo.setTwitter(wearerTwitter);

  if (displayingWearerDetails)
  {
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
