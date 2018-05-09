/*
 *      Copyright 2018 Particle Industries, Inc.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 *
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

#include "parti-badge.h" // #define pin assignments
#include "tones.h" // Peizo Sounds

Debounce displayDebouncer = Debounce();
Debounce gameDebouncer = Debounce();

// Wearer details
String wearerName;
String wearerEmail;
String wearerHandle;

// Default to display mode, but we'll determine this based on a switch
int badgeMode = DISPLAY_MODE;

void setup() {
  // Serial.begin(9600);

  cloudInit();

  pinMode(BUZZER_PIN, OUTPUT);
  displayDebouncer.attach(DISPLAY_MODE_PIN, INPUT_PULLDOWN);
  displayDebouncer.interval(20);

  gameDebouncer.attach(GAME_MODE_PIN, INPUT_PULLDOWN);
  gameDebouncer.interval(20);

  // Play a startup sound on the Piezo
  playStartup(BUZZER_PIN);
}

void loop() {
  // Check the switch to see if the user has changed the badge mode
  checkBadgeMode();
}

void cloudInit() {
  Particle.variable("wearerName", wearerName);
  Particle.variable("wearerEmail", wearerEmail);
  Particle.variable("wearerHandle", wearerHandle);

  Particle.subscribe("updateName", updateNameHandler);
  Particle.subscribe("updateEmail", updateEmailHandler);
  Particle.subscribe("updateHandle", updateHandleHandler);
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
