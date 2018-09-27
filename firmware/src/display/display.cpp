#include "Adafruit_SSD1306.h"

#include "macros.h"
#include "images/spark.h"
#include "images/spectra.h"
#include "images/temp.h"
#include "images/humidity.h"
#include "images/konami.h"
#include "images/mesh.h"
#include "images/battery.h"
#include "display.h"

extern Adafruit_SSD1306 display;
extern String wearerFirstName;
extern String wearerLastName;
extern String wearerTwitter;
extern bool titleShown;
extern int currentTemp;
extern int currentHumidity;
extern int currentBatteryReading;

bool displayingTemp = false;
bool displayingBattery = false;
bool displayingWearerDetails = false;
bool displayingCarousel = false;

// Array of function pointers so we can rotate through each in carousel mode
void (*carouselFuncs[])(void) = {
    showTitle,
    showSpark,
    showTempAndHumidity,
    showSpectra,
    displayWearerDetails};
int funcArraySize = sizeof(carouselFuncs) / sizeof(carouselFuncs[0]);
int currentFunc = 0;
unsigned long prevCarouselRotation;

void displayCarousel()
{
  unsigned long currentMillis = millis();

  if (!displayingCarousel)
  {
    currentFunc = 0;
    prevCarouselRotation = millis();
    carouselFuncs[currentFunc]();
    displayingCarousel = true;
  }
  else if (currentMillis - prevCarouselRotation > CAROUSEL_INTERVAL)
  {
    prevCarouselRotation = currentMillis;
    carouselFuncs[currentFunc]();
    currentFunc++;

    if (currentFunc == funcArraySize)
    {
      currentFunc = 0;
    }
  }
}

// Reset all display-related state booleans
void resetDisplayBools()
{
  displayingTemp = false;
  displayingWearerDetails = false;
  displayingCarousel = false;
  displayingBattery = false;
}

// Clear the OLED display
void clearScreen()
{
  display.stopscroll();
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.setTextWrap(true);
}

int getCenterX(const char text[], int textSize)
{
  int textWidth = strlen(text) * 9 * textSize;

  return 64 - (textWidth / 2);
}

void messageBoxWithArray(const char *textStrings[], int arrayLen, int textSize)
{
  int textHeight = 12;
  int y = textSize == 1 ? 64 / arrayLen : 18;
  clearScreen();
  display.setTextSize(textSize);

  for (int i = 0; i < arrayLen; i++)
  {
    int x = getCenterX(textStrings[i], textSize);

    display.setCursor(x + (15 / textSize) + i, y + textHeight * i * textSize);
    display.println(textStrings[i]);
  }
  display.display();
}

void showSplashscreen()
{
  clearScreen();
  display.drawBitmap(0, 16, sparkLogo, 128, 48, 1);
  display.display();
  delay(2000);
  clearScreen();
  display.drawBitmap(0, 0, spectraLogo, 128, 64, 1);
  display.display();
  delay(2000);
}

// Display the wearer's first and last name on the display
void displayWearerDetails()
{
  displayingWearerDetails = true;

  int fnameLength = wearerFirstName.length();
  int lnameLength = wearerLastName.length();
  int longestLength = ((fnameLength > lnameLength) ? fnameLength : lnameLength);

  if (fnameLength > 0 || lnameLength > 0)
  {
    clearScreen();
    //put Twitter info up in yellow band area
    if (wearerTwitter.length() > 10)
    {
      display.setTextSize(1);
    }
    if (wearerTwitter.length() > 0)
    {
      display.println(wearerTwitter);
    }

    // setTextSize based on largest of two lengths
    // Display is 128 x 64
    // So if the longest of the two names is longer than 10 characters,
    // set the size to 1
    if (longestLength > 10)
    {
      display.setTextSize(1);
      display.setCursor(0, 20);
    }
    else
    {
      display.setTextSize(2);
      display.setCursor(0, 10);
    }

    display.println();

    if (wearerFirstName.length() > 0)
    {
      display.println(wearerFirstName);
    }
    if (wearerLastName.length() > 0)
    {
      display.println(wearerLastName);
    }

    display.display();
    display.startscrollleft(0x00, 0x0F);
  }
}

void displayTwitterHandle()
{
  displayingWearerDetails = true;

  clearScreen();
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println();

  if (wearerTwitter.length() > 0)
  {
    display.println(wearerTwitter);
  }

  display.display();
  display.startscrollleft(0x00, 0x0F);
}

// Show the title text on the display
void showTitle()
{
  display.clearDisplay();
  display.setTextWrap(true);
  display.setTextColor(WHITE);
  display.setTextSize(2);

  display.setCursor(1, 18);
  display.println("PartiBadge");
  display.setCursor(0, 35);
  display.println("Spectra 18");
  display.display();
}

void showBattery()
{
  clearScreen();

  display.setTextSize(1);
  display.setCursor(40, 16);
  display.println("Battery");
  display.setTextSize(2);
  display.setCursor(37, 30);
  display.print((int)currentBatteryReading);
  display.println("%");

  // Show battery image based on reading
  int battX = 20;
  int battY = 40;
  if (currentBatteryReading > 0 && currentBatteryReading <= 20)
  {
    display.drawBitmap(battX, battY, batteryEmptyImage, 72, 32, 1);
  }
  else if (currentBatteryReading > 20 && currentBatteryReading <= 50)
  {
    display.drawBitmap(battX, battY, batteryLowImage, 72, 32, 1);
  }
  else if (currentBatteryReading > 50 && currentBatteryReading <= 90)
  {
    display.drawBitmap(battX, battY, batteryMediumImage, 72, 32, 1);
  }
  else
  {
    display.drawBitmap(battX, battY, batteryFullImage, 72, 32, 1);
  }

  display.display();
  display.setTextSize(1);

  displayingBattery = true;
}

// Show the temperature and humidity on the display
void showTempAndHumidity()
{
  clearScreen();

  display.drawBitmap(7, 18, tempImage, 16, 43, 1);
  display.setTextSize(1);
  display.setCursor(52, 16);
  display.println("Temp");
  display.setTextSize(2);
  display.setCursor(48, 25);
  display.print((int)currentTemp);
  display.println("f");
  display.setTextSize(1);
  display.setCursor(42, 42);
  display.println("Humidity");
  display.setTextSize(2);
  display.setCursor(48, 50);
  display.print((int)currentHumidity);
  display.println("%");
  display.drawBitmap(105, 28, humidityImage, 20, 27, 1);
  display.display();
  display.setTextSize(1);

  displayingTemp = true;
}

void showSpark()
{
  clearScreen();
  display.drawBitmap(0, 16, sparkLogo, 128, 48, 1);
  display.display();
  display.startscrollleft(0x00, 0x0F);
}

void displayCredits()
{
  const char *creditStrings[] = {"Created by",
                                 "a GLOBAL team", "at Particle!"};

  messageBoxWithArray(creditStrings, 3, 1);
}

void showKonami()
{
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.drawBitmap(0, 0, konamiLogo, 128, 64, 1);
  display.display();
  display.startscrolldiagleft(0x00, 0x07);
}

void showMesh()
{
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.drawBitmap(0, 0, meshLogo, 64, 63, 1);
  display.display();
  display.startscrolldiagright(0x00, 0x07);
}

void showSpectra()
{
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.drawBitmap(0, 0, spectraLogo, 128, 64, 1);
  display.display();
  display.startscrollleft(0x00, 0x0F);
}

void displayMeshPing()
{
  clearScreen();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println();
  display.println("You got a mesh ping!");
  display.display();
}