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

#include "parti-badge.h" // #define pin assignments and other general macros
#include "music/tones.h" // Peizo Sounds

// Custom code for Si7021 Temp/Hu Sensor using Wire1 on Electron C4, C5
#include "Si7021_MultiWire/Si7021_MultiWire.h"

// TFT include
#include "Adafruit_ST7735.h"

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

// Battery charge variables
FuelGauge fuel;
int currentBatteryCharge;

// Timing variables
unsigned long elapsedTime;
unsigned long startTime = 0;
unsigned long previousEnvReading = 0;
unsigned long previousBattReading = 0;

// Wearer details
String wearerName;
String wearerEmail;
String wearerHandle;

// Default to display mode, but we'll determine this based on a switch
int badgeMode = STARTUP_MODE;

// Display variables
bool displayingTemp = false;
bool displayingBatteryLevel = false;
bool displayingLogo = false;
bool displayingTitle = false;
bool displayingWearerDetails = false;

void setup() {
  // Play a startup sound on the Piezo
  playStartup(BUZZER_PIN);

  //Init TFT
  initDisplay();

  //Initialize Temp and Humidity sensor
  envSensor.begin();

  cloudInit();

  pinMode(BUZZER_PIN, OUTPUT);
  displayDebouncer.attach(DISPLAY_MODE_PIN, INPUT_PULLDOWN);
  displayDebouncer.interval(DEBOUNCE_DELAY);

  gameDebouncer.attach(GAME_MODE_PIN, INPUT_PULLDOWN);
  gameDebouncer.interval(DEBOUNCE_DELAY);

  //Init Tactile LED Buttons
  initLEDButtons();

  // Get an initial temp and humidity reading
  getTempAndHumidity();

  // Perform an initial battery check
  checkBattery();
}

void loop() {
  unsigned long currentMillis = millis();

  // Check the switch to see if the user has changed the badge mode
  checkBadgeMode();

  if (badgeMode == STARTUP_MODE) {
    if (currentMillis < LOGO_DURATION && !displayingLogo) {
      showLogo();
      displayingLogo = true;
    }
    if (currentMillis > LOGO_DURATION && !displayingTitle) {
      showTitle();
      displayingTitle = false;
    }
    if (currentMillis > TITLE_SCREEN_DURATION) {
      clearScreen();
      badgeMode = DISPLAY_MODE;
    }
  } else if (badgeMode == DISPLAY_MODE) {
    // TODO: Display Wearer Details

    if (! digitalRead(RED_BUTTON_A) && ! displayingTemp) {
      resetDisplayBools();
      displayingTemp = true;
      toggleAllButtons(LOW);
      digitalWrite(RED_LED, HIGH);

      // Show Temp and Humidity on Display
      showTempAndHumidity();
    }

    if (! digitalRead(BLUE_BUTTON_B) && ! displayingBatteryLevel) {
      resetDisplayBools();
      displayingBatteryLevel = true;
      toggleAllButtons(LOW);
      digitalWrite(BLUE_LED, HIGH);

      // Show Battery Level
      showBatteryLevel();
    }

    if (! digitalRead(GREEN_BUTTON_C)) {
      toggleAllButtons(LOW);
      digitalWrite(GREEN_LED, HIGH);

      clearScreen();
    }

    if (! digitalRead(YELLOW_BUTTON_D)) {
      toggleAllButtons(LOW);
      digitalWrite(YELLOW_LED, HIGH);

      clearScreen();
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
    // TODO: Intialize Game mode
  }
}

void cloudInit() {
  Particle.variable("wearerName", wearerName);
  Particle.variable("wearerEmail", wearerEmail);
  Particle.variable("wearerHandle", wearerHandle);

  Particle.variable("currentTemp", currentTemp);
  Particle.variable("currentHu", currentHumidity);

  Particle.variable("battCharge", currentBatteryCharge);

  Particle.subscribe("updateName", updateNameHandler);
  Particle.subscribe("updateEmail", updateEmailHandler);
  Particle.subscribe("updateHandle", updateHandleHandler);
}

void initDisplay() {
  display.initG();
  display.setRotation(3);

  pinMode(TFT_LIGHT, OUTPUT);
  digitalWrite(TFT_LIGHT, HIGH);
}

void showLogo() {
  // TODO: Show the Spark Logo for a few sec
}

void showTitle() {
  display.fillScreen(ST7735_WHITE);
  display.setCursor(0, 0);
  display.setTextColor(ST7735_RED);
  display.setTextWrap(true);
  display.setTextSize(2);

  display.println();
  display.println();
  display.println(" #PartiBadge");
  display.println(" v1.0");
  display.println();
  display.println(" BAMF Edition");
}

void initLEDButtons() {
  int del = 300;
  int medDel = 500;

  // Init LEDs and Outputs
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  // Init Buttons as Inputs
  redButtonADebouncer.attach(RED_BUTTON_A, INPUT_PULLUP);
  redButtonADebouncer.interval(DEBOUNCE_DELAY);
  blueButtonBDebouncer.attach(BLUE_BUTTON_B, INPUT_PULLUP);
  blueButtonBDebouncer.interval(DEBOUNCE_DELAY);
  greenButtonCDebouncer.attach(GREEN_BUTTON_C, INPUT_PULLUP);
  greenButtonCDebouncer.interval(DEBOUNCE_DELAY);
  yellowButtonDDebouncer.attach(YELLOW_BUTTON_D, INPUT_PULLUP);
  yellowButtonDDebouncer.interval(DEBOUNCE_DELAY);

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
  display.println("Curr Temp");
  display.print((int)currentTemp);
  display.println("f");
  display.println();
  display.println("Humidity");
  display.print((int)currentHumidity);
  display.println("%");
}

void showBatteryLevel() {
  clearScreen();

  display.setTextSize(4);
  display.println();
  display.println("BATT");
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

void updateNameHandler(const char *event, const char *data) {
  if (String(event) == "updateName") {
    wearerName = String(data);
  }
}

void updateEmailHandler(const char *event, const char *data) {
  if (String(event) == "updateEmail") {
    wearerEmail = String(data);
  }
}
void updateHandleHandler(const char *event, const char *data) {
  if (String(event) == "updateHandle") {
    wearerHandle = String(data);
  }
}
