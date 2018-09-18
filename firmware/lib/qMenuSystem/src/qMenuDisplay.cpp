#include "qMenuDisplay.h"

qMenuDisplay::qMenuDisplay()
{
}

qMenuDisplay::qMenuDisplay(Adafruit_SSD1306 *disp)
{
  _disp = disp;
}

void qMenuDisplay::Begin()
{
  _disp->stopscroll();
  _disp->clearDisplay();
  _disp->setCursor(0, 0);
  _disp->setTextWrap(true);
  _disp->setTextColor(WHITE);
  _disp->setTextSize(1);
}

void qMenuDisplay::Start()
{
  _disp->stopscroll();
  _disp->clearDisplay();
  _disp->setTextColor(WHITE);
}

void qMenuDisplay::Finish()
{
  // _disp->display();
}

void qMenuDisplay::Title(const char text[])
{
  _disp->setCursor(0, 11 - textHeight / 2);
  _disp->setTextColor(WHITE);
  _disp->println(text);
  _disp->drawFastHLine(0, 14, 128, WHITE);
  _disp->display();
}

void qMenuDisplay::Item(int index, const char text[])
{
  _disp->setCursor(5, (index * 14) + 32 - textHeight / 2);
  _disp->setTextColor(WHITE);
  _disp->println(text);
  _disp->display();
}

void qMenuDisplay::Highlight(int index)
{
  _disp->drawRect(0, (index * 14) + 22, 95, 13, WHITE);
  _disp->display();
}

void qMenuDisplay::MessageBox(const char text[])
{
  int textWidth = strlen(text) * 9;
  int x = 64 - (textWidth / 2);
  int y = 32;
  _disp->clearDisplay();

  _disp->setCursor(x + 10, y + 5 - textHeight / 2);
  _disp->println(text);
  _disp->display();
}
