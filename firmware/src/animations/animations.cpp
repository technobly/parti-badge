#include "Adafruit_SSD1306.h"
#include "interrupts/interrupts.h"
#include "display/display.h"

extern Adafruit_SSD1306 display;
extern byte appmode;
extern byte btncounter;
extern byte btnid;

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16
static const unsigned char logo16_glcd_bmp[] =
    {0B00000000, 0B00000000, 0B00000000, 0B00000000, 0B00000001, 0B00000000, 0B00010001, 0B10001000, 0B00001101, 0B00110000, 0B00001110, 0B11100000, 0B00000111, 0B11100000, 0B00111011, 0B11011100, 0B00010111, 0B11011000, 0B00000111, 0B11100000, 0B00001110, 0B01110000, 0B00001001, 0B10110000, 0B00010001, 0B10001000, 0B00000001, 0B00000000, 0B00000000, 0B00000000, 0B00000000, 0B00000000};

void drawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h)
{
  uint8_t icons[NUMFLAKES][3];
  appmode = 1;
  btnid = 0;

  // initialize
  for (uint8_t f = 0; f < NUMFLAKES; f++)
  {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
  }

  setupLEDButtonInterrupt();

  while (appmode)
  {
    // draw each icon
    for (uint8_t f = 0; f < NUMFLAKES; f++)
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
    }
    display.display();
    delay(200);

    // then erase it + move it
    for (uint8_t f = 0; f < NUMFLAKES; f++)
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height())
      {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
  appmode = 0;
  btncounter++;
}

void snowflakes()
{
  clearScreen();
  // draw a bitmap icon and 'animate' movement
  drawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

void drawcircle(void)
{
  clearScreen();

  for (int16_t i = 0; i < display.height(); i += 2)
  {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
  }
}

void drawCircles()
{
  appmode = 1;
  btnid = 0;
  setupLEDButtonInterrupt();

  while (appmode)
  {
    drawcircle();
  }
}

void drawtriangle(void)
{
  clearScreen();

  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5)
  {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, WHITE);
    display.display();
  }
}

void filltriangle(void)
{
  clearScreen();

  uint8_t color = WHITE;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5)
  {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, WHITE);
    if (color == WHITE)
      color = BLACK;
    else
      color = WHITE;
    display.display();
  }
}

void drawTriangles()
{
  appmode = 1;
  btnid = 0;
  setupLEDButtonInterrupt();

  while (appmode)
  {
    drawtriangle();
    delay(1000);
    filltriangle();
  }
}

void drawroundrect(void)
{
  clearScreen();

  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
  {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, WHITE);
    display.display();
  }
}

void fillroundrect(void)
{
  clearScreen();

  uint8_t color = WHITE;
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
  {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
    if (color == WHITE)
      color = BLACK;
    else
      color = WHITE;
    display.display();
  }
}

void drawRoundedRectangles()
{
  appmode = 1;
  btnid = 0;
  setupLEDButtonInterrupt();

  while (appmode)
  {
    drawroundrect();
    fillroundrect();
  }
}

void fillrect(void)
{
  clearScreen();

  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3)
  {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    color++;
  }
}

void drawrect(void)
{
  clearScreen();

  for (int16_t i = 0; i < display.height() / 2; i += 2)
  {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
    display.display();
  }
}

void drawRectangles()
{
  appmode = 1;
  btnid = 0;
  setupLEDButtonInterrupt();

  while (appmode)
  {
    drawrect();
    fillrect();
  }
}

void scrolltext()
{
  clearScreen();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println("#Parti");
  display.println("Badge");
  display.display();

  display.startscrollright(0B00000000, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0B00000000, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0B00000000, 0B00000111);
  delay(2000);
  display.startscrolldiagleft(0B00000000, 0B00000111);
  delay(2000);
  display.stopscroll();
}

void scrollText(void)
{
  appmode = 1;
  btnid = 0;
  setupLEDButtonInterrupt();

  while (appmode)
  {
    scrolltext();
  }
}

void cycleAnimations()
{
  appmode = 1;
  btnid = 0;
  setupLEDButtonInterrupt();

  while (appmode)
  {
    clearScreen();

    // draw rectangles
    drawrect();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw multiple rectangles
    fillrect();
    display.display();
    delay(2000);
    display.clearDisplay();

    // draw mulitple circles
    drawcircle();
    display.display();
    delay(2000);
    display.clearDisplay();

    drawroundrect();
    delay(2000);
    display.clearDisplay();

    fillroundrect();
    delay(2000);
    display.clearDisplay();

    drawtriangle();
    delay(2000);
    display.clearDisplay();

    filltriangle();
    delay(2000);
    display.clearDisplay();

    // draw scrolling text
    scrolltext();
    delay(2000);
    display.clearDisplay();

    // invert the display
    display.invertDisplay(true);
    delay(1000);
    display.invertDisplay(false);
    delay(1000);
    display.clearDisplay();
    display.display();

    snowflakes();
  }
}
