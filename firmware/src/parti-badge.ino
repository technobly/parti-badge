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
 *              6. 4 Tactile LED Buttons in Red, Blue, Green and Yello/Orange
 *              7. An I2C-Compatible breakout for #BadgeLife add-ons
 *
 */

#include "Particle.h"
#include "Debounce.h"

#include "parti-badge.h" // #define pin assignments and other general macros
#include "music/tones.h" // Peizo Sounds
#include "music/roll.h"

#include "images/spark.h"

#include "Adafruit_Si7021.h"

// Display includes
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

PRODUCT_ID(7775);
PRODUCT_VERSION(18);

String deviceId;

// Button Debounce Support
Debounce redButtonADebouncer = Debounce();
Debounce blueButtonBDebouncer = Debounce();
Debounce greenButtonCDebouncer = Debounce();
Debounce yellowButtonDDebouncer = Debounce();
#define DEBOUNCE_DELAY 20

// Initialize Si7021 sensor
Adafruit_Si7021 envSensor = Adafruit_Si7021();
int currentTemp;
int currentHumidity;

// Init Display
Adafruit_SSD1306 display(RESET);

#include "simonsays/simon.h" // Simon Says Code

// Timing variables
unsigned long elapsedTime;
unsigned long startTime = 0;
unsigned long previousEnvReading = 0;

// Wearer details
String wearerFirstName;
String wearerLastName;

// Default to display mode, but we'll determine this based on a
// the user holding a button down at startup
int badgeMode = DISPLAY_MODE;

// Display variables
bool displayingTemp = false;
bool displayingLogo = false;
bool displayingTitle = false;
bool displayingWearerDetails = false;

// Display state management
bool titleShown = false;
bool buttonsInitialized = false;

void setup() {
  resetDisplayBools();

  // Get the current deviceId
  deviceId = System.deviceID();

  //Initialize Temp and Humidity sensor
  while(! envSensor.begin());

  //Init OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Show Particle Splashscreen
  showSplashscreen();

  // Show the title screen
  showTitle();

  // Set up cloud variables and functions
  cloudInit();

  rollSetup();

  pinMode(BUZZER_PIN, OUTPUT);

  // Init the LED Buttons
  initButtons();

  // Get an initial temp and humidity reading
  getTempAndHumidity();

  //Init Tactile LED Buttons
  initLEDButtons();

  // Play a startup sound on the Piezo
  if (!startupSoundPlayed) playStartup(BUZZER_PIN);

  checkBadgeMode();

  Particle.connect();

  // Scroll the title text on the screen
  display.startscrollleft(0x00, 0x0F);
}

void loop() {
  unsigned long currentMillis = millis();

  if (badgeMode == DISPLAY_MODE) {
    redButtonADebouncer.update();
    if (redButtonADebouncer.read() == LOW) {
      resetDisplayBools();

      toggleAllButtons(LOW);
      digitalWrite(RED_LED, HIGH);
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
    if (greenButtonCDebouncer.read() == LOW) {
      resetDisplayBools();

      toggleAllButtons(LOW);
      digitalWrite(GREEN_LED, HIGH);

      // Show Name
      // showName();
      // delay(1000);
      initButtons();
      attractMode();
    }

    yellowButtonDDebouncer.update();
    if (yellowButtonDDebouncer.read() == LOW) {
      // wearerDetailsTriggerTime = millis();

      resetDisplayBools();
      // displayingWearerDetails = true;

      toggleAllButtons(LOW);
      digitalWrite(YELLOW_LED, HIGH);
    }

    if (currentMillis - previousEnvReading > TEMP_CHECK_INTERVAL) {
      previousEnvReading = currentMillis;
      getTempAndHumidity();
    }

    /* Some other secret key combo to trigger the roll */
    // if (secredKeyComboFound) {
    //  playRoll();
    //}
  } else if (badgeMode == GAME_MODE) {
    configureGame();

    playGame();
  }
}


void showSplashscreen() {
  clearScreen();
  display.drawBitmap(0, 0, sparkLogo, 128, 64, 1);
  display.display();
  delay(3000);
}

void checkBadgeMode() {
  redButtonADebouncer.update();
  blueButtonBDebouncer.update();

  if (blueButtonBDebouncer.read() == LOW && blueButtonBDebouncer.read() == LOW) {
    badgeMode = GAME_MODE;
  } else {
    badgeMode = DISPLAY_MODE;
  }
}

void cloudInit() {
  Particle.variable("wearerFName", wearerFirstName);
  Particle.variable("wearerLName", wearerLastName);

  Particle.variable("currentTemp", currentTemp);
  Particle.variable("currentHu", currentHumidity);

  Particle.function("updateFName", updateFirstNameHandler);
  Particle.function("updateLName", updateLastNameHandler);

  Particle.function("checkTemp", checkTempHandler);
}

void showTitle() {
  titleShown = true;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextWrap(true);
  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.println();
  display.println();
  display.println(" #PartiBadge v2");
  display.println();
  display.println(" THAT Conference 2018");
  display.display();
}

void displayWearerDetails() {
  /*
  display.fillScreen(ST7735_WHITE);
  display.setCursor(0, 0);
  display.setTextColor(ST7735_BLUE);
  display.setTextWrap(true);
  display.setTextSize(3);

  display.println();
  display.println(wearerFirstName);
  display.println(wearerLastName);
  */
}

void showName() {
  /*
  display.fillScreen(ST7735_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(ST7735_WHITE);
  display.setTextWrap(true);
  display.setTextSize(3);

  display.println();
  display.println(" Brandon");
  display.println("");
  display.println(" Particle");
  */
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

  display.setTextSize(1);
  display.println();
  display.println("         Temp");
  display.setTextSize(2);
  display.print("    ");
  display.print((int)currentTemp);
  display.println("f");
  display.setTextSize(1);
  display.println();
  display.println("       Humidity");
  display.setTextSize(2);
  display.print("    ");
  display.print((int)currentHumidity);
  display.println("%");
  display.display();
}

void toggleAllButtons(int state) {
  digitalWrite(RED_LED, state);
  digitalWrite(BLUE_LED, state);
  digitalWrite(GREEN_LED, state);
  digitalWrite(YELLOW_LED, state);
}

void resetDisplayBools() {
  displayingTemp = false;
  displayingWearerDetails = false;
  displayingLogo = false;
  displayingTitle = false;
}

void getTempAndHumidity() {
  int prevTemp = currentTemp;
  int prevHumidity = currentHumidity;

  currentTemp = round((envSensor.readTemperature() * 1.8 + 32.00)*10)/10;
  currentHumidity = round(envSensor.readHumidity()*10)/10;

  // If either has changed and these values are being displayed, update the display
  if (displayingTemp && (prevTemp != currentTemp || prevHumidity != currentHumidity)) {
    showTempAndHumidity();
  }
}

void clearScreen() {
  display.stopscroll();
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.setTextWrap(true);
}

int updateFirstNameHandler(String data) {
  wearerFirstName = data;

  return 1;
}

int updateLastNameHandler(String data) {
  wearerLastName = data;

  return 1;
}

int checkTempHandler(String data) {
  getTempAndHumidity();

  return 1;
}
