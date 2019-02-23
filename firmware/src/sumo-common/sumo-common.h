#pragma once

#define BTN_NONE (0x0000) // no buttons pressed
#define BTN_UP   (0x0001) // blue button UP
#define BTN_DN   (0x0002) // yellow button DOWN
#define BTN_LF   (0x0004) // red button LEFT
#define BTN_RT   (0x0008) // green button RIGHT
#define JOY_UP   (0x0010) // joystick UP
#define JOY_DN   (0x0020) // joystick DOWN
#define JOY_LF   (0x0040) // joystick LEFT
#define JOY_RT   (0x0080) // joystick RIGHT

extern Adafruit_SSD1306 display;

void checkInput(int &buttons);
void drawJoyLeft(int x, int y, bool visible);
void drawJoyDown(int x, int y, bool visible);
void drawJoyUp(int x, int y, bool visible);
void drawJoyRight(int x, int y, bool visible);
void drawButton(int x, int y, const char* c, bool visible);
void displayHeader(const char * header);
void displayFooter(const char * footer);
void displaySetup(const char * header, const char * footer);
void displayLoop(int &buttons);
