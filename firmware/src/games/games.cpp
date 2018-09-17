#include "Adafruit_SSD1306.h"
#include "inputs/inputs.h"
#include "display/display.h"
#include "interrupts/interrupts.h"

extern Adafruit_SSD1306 display;
extern byte appmode;
extern byte btncounter;
extern byte btnid;

void drawFilledCircle(int displayX, int displayY)
{
  display.drawCircle(displayX, displayY, 1, WHITE);
  display.drawCircle(displayX, displayY, 2, WHITE);
  display.drawCircle(displayX, displayY, 3, WHITE);
  display.drawCircle(displayX, displayY, 4, WHITE);
  display.drawCircle(displayX, displayY, 5, WHITE);
  display.display();
}

// Init Etch A Sketch mode on the OLED
void initEtchASketch()
{
  clearScreen();
  display.setTextSize(2);
  display.setCursor(1, 18);
  display.println("  Etch A");
  display.println("  Sketch");
  display.setTextSize(1);
  display.println("Use the joystick...");
  display.display();
  delay(2000);

  clearScreen();
}

// Update the display during Etch A Sketch Mode. Draw a new filled circle
// based on the which joystick direction was used.
void etchASketch()
{
  appmode = 1;
  btnid = 0;
  int displayX = display.width() / 2;
  int displayY = display.height() / 2;

  initEtchASketch();
  drawFilledCircle(displayX, displayY);

  setupAButtonInterrupt();

  while (appmode) {
    int lastY = displayY;
    int lastX = displayX;

    joystickUpDebouncer.update();
    if (joystickUpDebouncer.read() == LOW)
    {
      displayY--;
    }

    joystickDownDebouncer.update();
    if (joystickDownDebouncer.read() == LOW)
    {
      displayY++;
    }

    joystickLeftDebouncer.update();
    if (joystickLeftDebouncer.read() == LOW)
    {
      displayX--;
    }

    joystickRightDebouncer.update();
    if (joystickRightDebouncer.read() == LOW)
    {
      displayX++;
    }

    // Reset the screen
    joystickCenterDebouncer.update();
    if (joystickCenterDebouncer.read() == LOW)
    {
      clearScreen();
      displayX = display.width() / 2;
      displayY = display.height() / 2;
    }

    if ((lastX != displayX) || (lastY != displayY))
    {
      drawFilledCircle(displayX, displayY);
    }
  }
}
