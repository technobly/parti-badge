/**
 * Particle Mesh Sumo Bot
 * Sumo Common Routines
 * Author: Brett Walach
 * Date: 2019-02-19
 */

#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include "macros.h"
#include "display/display.h"
#include "interrupts/interrupts.h"
#include "inputs/inputs.h"
#include "leds/leds.h"
#include "music/music.h"
#include "sumo-common/sumo-common.h"

extern Adafruit_SSD1306 display;

void checkInput(int &buttons) {
    buttons = 0;
    // Debounce
    redButtonADebouncer.update();
    greenButtonCDebouncer.update();
    blueButtonBDebouncer.update();
    yellowButtonDDebouncer.update();
    joystickUpDebouncer.update();
    joystickDownDebouncer.update();
    joystickLeftDebouncer.update();
    joystickRightDebouncer.update();
    // Button LEFT?
    if (redButtonADebouncer.read() == LOW) {
        buttons |= BTN_LF;
    }
    // Button RIGHT?
    if (greenButtonCDebouncer.read() == LOW) {
        buttons |= BTN_RT;
    }
    // Button UP?
    if (blueButtonBDebouncer.read() == LOW) {
        buttons |= BTN_UP;
    }
    // Button DOWN?
    if (yellowButtonDDebouncer.read() == LOW) {
        buttons |= BTN_DN;
    }
    // Joystick UP?
    if (joystickUpDebouncer.read() == LOW) {
        buttons |= JOY_UP;
    }
    // Joystick DOWN?
    if (joystickDownDebouncer.read() == LOW) {
        buttons |= JOY_DN;
    }
    // Joystick LEFT?
    if (joystickLeftDebouncer.read() == LOW) {
        buttons |= JOY_LF;
    }
    // Joystick RIGHT?
    if (joystickRightDebouncer.read() == LOW) {
        buttons |= JOY_RT;
    }
    // Serial.printlnf("Buttons: %d", buttons);
}

void drawJoyLeft(int x, int y, bool visible) {
    display.setCursor(x, y);
    display.drawFastHLine(x + 4, y + 8, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 3, y + 7, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 2, y + 6, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 1, y + 5, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 0, y + 4, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 1, y + 3, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 2, y + 2, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 3, y + 1, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 4, y + 0, 3, (visible ? WHITE : BLACK));
}

void drawJoyRight(int x, int y, bool visible) {
    display.setCursor(x, y);
    display.drawFastHLine(x + 0, y + 8, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 1, y + 7, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 2, y + 6, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 3, y + 5, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 4, y + 4, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 3, y + 3, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 2, y + 2, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 1, y + 1, 3, (visible ? WHITE : BLACK));
    display.drawFastHLine(x + 0, y + 0, 3, (visible ? WHITE : BLACK));
}

void drawJoyDown(int x, int y, bool visible) {
    display.setCursor(x, y);
    display.drawFastVLine(x + 0, y + 0, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 1, y + 1, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 2, y + 2, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 3, y + 3, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 4, y + 4, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 5, y + 3, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 6, y + 2, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 7, y + 1, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 8, y + 0, 3, (visible ? WHITE : BLACK));
}

void drawJoyUp(int x, int y, bool visible) {
    display.setCursor(x, y);
    display.drawFastVLine(x + 0, y + 4, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 1, y + 3, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 2, y + 2, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 3, y + 1, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 4, y + 0, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 5, y + 1, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 6, y + 2, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 7, y + 3, 3, (visible ? WHITE : BLACK));
    display.drawFastVLine(x + 8, y + 4, 3, (visible ? WHITE : BLACK));
}

void drawButton(int x, int y, const char* c, bool visible) {
    display.setCursor(x, y);
    display.println(c);
    display.drawCircle(x+2, y+3, 6, WHITE);
    display.drawCircle(x+2, y+3, 7, (visible ? WHITE : BLACK));
    display.drawCircle(x+2, y+3, 8, (visible ? WHITE : BLACK));
}

const int joyx = 20;
const int joyy = 36;

void displayHeader(const char * header) {
    static char last[24] = "";
    display.setCursor(0, 0);
    display.setTextSize(1);
    if (strlen(last) > 0) {
        display.setTextColor(BLACK);
        display.println(last);
    }
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.println(header);
    display.drawFastHLine(0, 14, 128, WHITE);
    strncpy(last, header, strlen(header));
}

void displayFooter(const char * footer) {
    static char last[24] = "";
    display.setCursor(0, 56);
    display.setTextSize(1);
    if (strlen(last) > 0) {
        display.setTextColor(BLACK);
        display.println(last);
    }
    display.setCursor(0, 56);
    display.setTextColor(WHITE);
    display.println(footer);
    display.drawFastHLine(0, 14, 128, WHITE);
    strncpy(last, footer, strlen(footer));
}

void displaySetup(const char * header, const char * footer) {
    // Header
    clearScreen();
    displayHeader(header);
    display.drawFastHLine(0, 14, 128, WHITE);
    // Joystick
    display.drawCircle(joyx, joyy, 5, WHITE);
    display.drawCircle(joyx, joyy, 8, WHITE);
    drawJoyLeft(joyx - 14, joyy - 4, true);
    drawJoyRight(joyx + 8, joyy - 4, true);
    drawJoyUp(joyx - 4, joyy - 14, true);
    drawJoyDown(joyx - 4, joyy + 8, true);
    drawJoyLeft(joyx - 16, joyy - 4, false);
    drawJoyRight(joyx + 10, joyy - 4, false);
    drawJoyUp(joyx - 4, joyy - 16, false);
    drawJoyDown(joyx - 4, joyy + 10, false);
    // Buttons
    drawButton(90, 21, "B",  false);
    drawButton(77, 33, "A",  false);
    drawButton(103, 33, "C", false);
    drawButton(90, 44, "D",  false);
    // Footer
    display.setCursor(0, 56);
    display.println(footer);
    display.display();
}

void displayLoop(int &buttons) {
    drawJoyLeft(joyx - 16, joyy - 4, buttons & JOY_LF);
    drawJoyRight(joyx + 10, joyy - 4, buttons & JOY_RT);
    drawJoyUp(joyx - 4, joyy - 16, buttons & JOY_UP);
    drawJoyDown(joyx - 4, joyy + 10, buttons & JOY_DN);
    drawButton(90, 21, "B",  buttons & BTN_UP);
    drawButton(77, 33, "A",  buttons & BTN_LF);
    drawButton(103, 33, "C", buttons & BTN_RT);
    drawButton(90, 44, "D",  buttons & BTN_DN);
    display.display();
}
