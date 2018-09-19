#ifndef qMenuDisplay_h
#define qMenuDisplay_h

#if !defined SSD1306_128_64
#include <Adafruit_SSD1306.h>
#endif

class qMenuDisplay
{
public:
  qMenuDisplay(Adafruit_SSD1306 *disp);
  qMenuDisplay();
  void Begin();
  void Start();
  void Finish();
  void Title(const char text[]);
  void Item(int index, const char text[]);
  void Highlight(int index);
  void MessageBox(const char text[]);

private:
  Adafruit_SSD1306 *_disp;
  int textHeight = 12;
};

#endif
