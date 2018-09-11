#include "Adafruit_SSD1306.h"
#include "images/spark.h"
#include "images/temp.h"
#include "images/humidity.h"

extern Adafruit_SSD1306 display;
extern String wearerFirstName;
extern String wearerLastName;
extern String wearerTwitter;
extern bool titleShown;
extern int currentTemp;
extern int currentHumidity;

// Clear the OLED display
void clearScreen()
{
  display.stopscroll();
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.setTextWrap(true);
}

void showSplashscreen() {
  clearScreen();
  display.drawBitmap(0, 16, sparkLogo, 128, 48, 1);
  display.display();
  delay(3000);
}

// Display the wearer's first and last name on the display
void displayWearerDetails()
{
  int fnameLength = wearerFirstName.length();
  int lnameLength = wearerLastName.length();
  int longestLength = ((fnameLength > lnameLength) ? fnameLength : lnameLength);

  if (fnameLength > 0 || lnameLength > 0)
  {
    clearScreen();
    //put Twitter info up in yellow band area
    if (wearerTwitter.length() > 10) {
      display.setTextSize(1);
    }
    if (wearerTwitter.length() > 0) {
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

void displayTwitterHandle() {
  clearScreen();
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println();

  if (wearerTwitter.length() > 0) {
    display.println(wearerTwitter);
  }

  display.display();
  display.startscrollleft(0x00, 0x0F);
}

// Show the title text on the display
void showTitle()
{
  titleShown = true;

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
}

void displayCredits() {
  clearScreen();

  display.setTextWrap(true);
  display.setCursor(1, 28);
  display.setTextSize(1);
  display.println("Created by");
  display.println("a GLOBAL team");
  display.println("at Particle!");
  display.display();
  display.startscrollleft(0x00, 0x0F);
}
