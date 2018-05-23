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
 *      Project: #PartiBadge, 2018 Maker Faire Edition
 *
 *      Description:
 *          This is the firmware for a custom, Electron-based badge PCB that includes:
 *              1. A 1.7" LCD TFT
 *              2. An SMD Piezo buzzer
 *              3. A SPDT Switch
 *              4. A 5-way joystick
 *              5. An SMD Si7021 temperature and Humidity sensor
 *              6. 4 Tactile LED Buttons in Red, Blue, Green and Yello/Orange
 *              7. An I2C-Compatible breakout for #BadgeLife add-ons
 *          The exact functionality of this badge includes:
 *              1.
 */

#include "Particle.h"
#include "Debounce.h"
#include "SdFat.h"

#include "parti-badge.h" // #define pin assignments and other general macros
#include "music/tones.h" // Peizo Sounds
#include "music/roll.h"

// Custom code for Si7021 Temp/Hu Sensor using Wire1 on Electron C4, C5
#include "Si7021_MultiWire/Si7021_MultiWire.h"

// TFT include
#include "Adafruit_ST7735.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

PRODUCT_ID(7461);
PRODUCT_VERSION(2);

String deviceId;

// Button Debounce Support
Debounce displayDebouncer = Debounce();
Debounce gameDebouncer = Debounce();
Debounce redButtonADebouncer = Debounce();
Debounce blueButtonBDebouncer = Debounce();
Debounce greenButtonCDebouncer = Debounce();
Debounce yellowButtonDDebouncer = Debounce();
#define DEBOUNCE_DELAY 20

// Initialize Si7021 sensor
Si7021_MultiWire envSensor = Si7021_MultiWire();
double currentTemp;
double currentHumidity;

// Initialize TFT Display
Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#include "simonsays/simon.h" // Simon Says Code

//SD Card
SdFat sd;

// Battery charge variables
FuelGauge fuel;
int currentBatteryCharge;

// Timing variables
unsigned long elapsedTime;
unsigned long startTime = 0;
unsigned long previousEnvReading = 0;
unsigned long previousBattReading = 0;

// Wearer details
String wearerFirstName;
String wearerLastName;

// Default to display mode, but we'll determine this based on a switch
int badgeMode = DISPLAY_MODE;
unsigned long meshImagesTriggerTime = 0;
unsigned long wearerDetailsTriggerTime = 0;
int meshImageArrayLength = 5;
int rollImageArrayLength = 5;
int currentMeshImage = 0;
int currentRollImage = 0;
char* meshImages[] = {
  "argon.bmp",
  "boron.bmp",
  "xenon.bmp",
  "xenonandboron.bmp",
  "xenongas.bmp"
};
char* rollImages[] = {
  "rick1.bmp",
  "rick2.bmp",
  "rick3.bmp",
  "rick4.bmp",
  "rick5.bmp"
};

// Display variables
bool displayingTemp = false;
bool displayingBatteryLevel = false;
bool displayingLogo = false;
bool displayingTitle = false;
bool displayingWearerDetails = false;
bool displayingMeshImages = false;

// Display state management
bool titleShown = false;
bool buttonsInitialized = false;

LEDStatus breatheCyan(RGB_COLOR_CYAN, LED_PATTERN_FADE, LED_PRIORITY_IMPORTANT);

void bmpDraw(char *filename, uint8_t x, uint16_t y);

void setup() {
  resetDisplayBools();

  // Get the current deviceId
  deviceId = System.deviceID();

  //Initialize Temp and Humidity sensor
  envSensor.begin();

  //Init TFT
  initDisplay();

  //Init SD
  sd.begin(TFT_SD_CS, SPI_FULL_SPEED);

  // Set up cloud variables and functions
  cloudInit();

  rollSetup();

  // Show the Particle Logo on the screen
  showLogo();

  pinMode(BUZZER_PIN, OUTPUT);
  displayDebouncer.attach(DISPLAY_MODE_PIN, INPUT_PULLDOWN);
  displayDebouncer.interval(DEBOUNCE_DELAY);

  gameDebouncer.attach(GAME_MODE_PIN, INPUT_PULLDOWN);
  gameDebouncer.interval(DEBOUNCE_DELAY);

  // Init the LED Buttons
  initButtons();

  // Get an initial temp and humidity reading
  getTempAndHumidity();

  // Perform an initial battery check
  checkBattery();

  // Show the title screen
  showTitle();

  //Init Tactile LED Buttons
  initLEDButtons();

  // Play a startup sound on the Piezo
  if (!startupSoundPlayed) playStartup(BUZZER_PIN);

  Particle.connect();
  //breatheCyan.setActive(true);

  checkBadgeMode();
  if (badgeMode == DISPLAY_MODE) {
    displayingMeshImages = true;
    meshImagesTriggerTime = millis();
  }
}

void loop() {
  unsigned long currentMillis = millis();

  checkBadgeMode();

  if (badgeMode == DISPLAY_MODE) {
    redButtonADebouncer.update();
    if (redButtonADebouncer.read() == LOW && ! displayingMeshImages) {
      meshImagesTriggerTime = millis();

      resetDisplayBools();
      displayingMeshImages = true;

      toggleAllButtons(LOW);
      digitalWrite(RED_LED, HIGH);

      clearScreen();
      bmpDraw(meshImages[currentMeshImage], 0, 0);
      currentMeshImage++;
    }

    blueButtonBDebouncer.update();
    if (blueButtonBDebouncer.read() == LOW && ! displayingTemp) {
      resetDisplayBools();
      displayingTemp = true;
      toggleAllButtons(LOW);
      digitalWrite(BLUE_LED, HIGH);

      // Show Temp and Humidity on Display
      showTempAndHumidity();
    }

    greenButtonCDebouncer.update();
    if (greenButtonCDebouncer.read() == LOW && ! displayingBatteryLevel) {
      resetDisplayBools();
      displayingBatteryLevel = true;
      toggleAllButtons(LOW);
      digitalWrite(GREEN_LED, HIGH);

      // Show Battery Level
      showBatteryLevel();
    }

    yellowButtonDDebouncer.update();
    if (yellowButtonDDebouncer.read() == LOW && !displayingWearerDetails) {
      wearerDetailsTriggerTime = millis();
      resetDisplayBools();
      displayingWearerDetails = true;

      toggleAllButtons(LOW);
      digitalWrite(YELLOW_LED, HIGH);

      clearScreen();
      bmpDraw(rollImages[currentRollImage], 0, 0);
      currentRollImage++;

      playRoll();
    }

    if (displayingWearerDetails) {
      playRoll();

      if (millis() - wearerDetailsTriggerTime > IMAGE_DURATION) {
        bmpDraw(rollImages[currentRollImage], 0, 0);
        if (currentRollImage == rollImageArrayLength) {
          currentRollImage = 0;
        } else {
          currentRollImage++;
        }
        wearerDetailsTriggerTime = millis();
      }
    }

    if (displayingMeshImages) {
      if (millis() - meshImagesTriggerTime > IMAGE_DURATION) {
        bmpDraw(meshImages[currentMeshImage], 0, 0);
        if (currentMeshImage >= meshImageArrayLength) {
          currentMeshImage = 0;
        } else {
          currentMeshImage++;
        }
        meshImagesTriggerTime = millis();
      }
    }

    if (currentMillis - previousEnvReading > TEMP_CHECK_INTERVAL) {
      previousEnvReading = currentMillis;
      getTempAndHumidity();
    }

    if (currentMillis - previousBattReading > BATT_CHECK_INTERVAL) {
      previousBattReading = currentMillis;
      checkBattery();
    }
  } else if (badgeMode == GAME_MODE) {
    configureGame();

    playGame();
  }
}

void cloudInit() {
  Particle.variable("wearerFName", wearerFirstName);
  Particle.variable("wearerLName", wearerLastName);

  Particle.variable("currentTemp", currentTemp);
  Particle.variable("currentHu", currentHumidity);

  Particle.variable("battCharge", currentBatteryCharge);

  Particle.function("updateFName", updateFirstNameHandler);
  Particle.function("updateLName", updateLastNameHandler);
}

void initDisplay() {
  display.initR(INITR_BLACKTAB);
  display.setCursor(0, 0);
  display.fillScreen(ST7735_BLACK);

  pinMode(TFT_LIGHT, OUTPUT);
  digitalWrite(TFT_LIGHT, HIGH);
}

void showLogo() {
  display.setCursor(0, 0);
  display.setRotation(3);

  bmpDraw("spark.bmp", 0, 0);
  delay(2000);
}

void showTitle() {
  titleShown = true;

  display.setRotation(3);
  display.fillScreen(ST7735_WHITE);
  display.setCursor(0, 0);
  display.setTextColor(ST7735_BLUE);
  display.setTextWrap(true);
  display.setTextSize(2);

  display.println();
  display.println();
  display.println(" #PartiBadge");
  display.println(" v1.0");
  display.println();
  display.println(" BAMF Edition");
}

void displayWearerDetails() {
  display.fillScreen(ST7735_WHITE);
  display.setCursor(0, 0);
  display.setTextColor(ST7735_BLUE);
  display.setTextWrap(true);
  display.setTextSize(3);

  display.println();
  display.println(wearerFirstName);
  display.println(wearerLastName);
}

void initButtons() {
  // Init Buttons as Inputs
  redButtonADebouncer.attach(RED_BUTTON_A, INPUT_PULLUP);
  redButtonADebouncer.interval(DEBOUNCE_DELAY);
  blueButtonBDebouncer.attach(BLUE_BUTTON_B, INPUT_PULLUP);
  blueButtonBDebouncer.interval(DEBOUNCE_DELAY);
  greenButtonCDebouncer.attach(GREEN_BUTTON_C, INPUT_PULLUP);
  greenButtonCDebouncer.interval(DEBOUNCE_DELAY);
  yellowButtonDDebouncer.attach(YELLOW_BUTTON_D, INPUT_PULLUP);
  yellowButtonDDebouncer.interval(DEBOUNCE_DELAY);
}

void initLEDButtons() {
  buttonsInitialized = true;

  int del = 300;
  int medDel = 500;

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

void showTempAndHumidity() {
  clearScreen();

  display.println();
  display.println("  Curr Temp");
  display.setTextSize(3);
  display.print("  ");
  display.print((int)currentTemp);
  display.println("f");
  display.setTextSize(2);
  display.println();
  display.println("  Humidity");
  display.setTextSize(3);
  display.print("  ");
  display.print((int)currentHumidity);
  display.println("%");
}

void showBatteryLevel() {
  clearScreen();

  display.setTextSize(4);
  display.println();
  display.println(" BATT");
  display.print("  ");
  display.print((int)currentBatteryCharge);
  display.println("%");
}

void toggleAllButtons(int state) {
  digitalWrite(RED_LED, state);
  digitalWrite(BLUE_LED, state);
  digitalWrite(GREEN_LED, state);
  digitalWrite(YELLOW_LED, state);
}

void resetDisplayBools() {
  displayingTemp = false;
  displayingBatteryLevel = false;
  displayingWearerDetails = false;
  displayingMeshImages = false;
  displayingLogo = false;
  displayingTitle = false;
}

void checkBadgeMode() {
  displayDebouncer.update();
  gameDebouncer.update();

  if (displayDebouncer.read() == HIGH) {
    badgeMode = DISPLAY_MODE;
  } else if (gameDebouncer.read() == HIGH) {
    badgeMode = GAME_MODE;
  }
}

void getTempAndHumidity() {
  currentTemp = envSensor.readTempF();
  currentHumidity = envSensor.getRH();
}

void checkBattery() {
  currentBatteryCharge = (int)fuel.getSoC();

  if (currentBatteryCharge < BATTERY_CRITICAL) {
    clearScreen();

    display.setTextSize(4);
    display.setTextColor(ST7735_BLUE);
    display.println();
    display.println("LOW BATT");
  }

  if (currentBatteryCharge < BATTERY_SHUTOFF) {
    playGameOver(BUZZER_PIN);

    //Sleep the device to prevent the battery from fully discharging
    System.sleep(SLEEP_MODE_SOFTPOWEROFF);
  }
}

void clearScreen() {
  display.fillScreen(ST7735_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(ST7735_WHITE);
  display.setTextWrap(true);
  display.setTextSize(2);
}

int updateFirstNameHandler(String data) {
  wearerFirstName = data;

  return 1;
}

int updateLastNameHandler(String data) {
  wearerLastName = data;

  return 1;
}

#include "bmpDraw.h" // Function for drawing Bitmaps on the screen
