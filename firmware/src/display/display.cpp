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

bool displayingTemp = false;
bool displayingWearerDetails = false;
bool displayingAnimations = false;

// Reset all display-related state booleans
void resetDisplayBools()
{
  displayingTemp = false;
  displayingWearerDetails = false;
  displayingAnimations = false;
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

int getCenterX(const char text[], int textSize) {
  int textWidth = strlen(text) * 9 * textSize;

  return 64 - (textWidth / 2);
}

void messageBoxWithArray(const char *textStrings[], int arrayLen, int textSize) {
  int textHeight = 12;
  int y = textSize == 1 ? 64 / arrayLen : 18;
  clearScreen();
  display.setTextSize(textSize);

  for (int i = 0; i < arrayLen; i++) {
    int x = getCenterX(textStrings[i], textSize);

    display.setCursor(x + (15 / textSize) + i, y + textHeight * i * textSize);
    display.println(textStrings[i]);
  }
  display.display();
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
  displayingWearerDetails = true;

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
  displayingWearerDetails = true;

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
  displayingTemp = true;
  
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

  displayingTemp = true;
}

void displayCredits() {
  const char *creditStrings[] = { "Created by",
    "a GLOBAL team", "at Particle!" };

  messageBoxWithArray(creditStrings, 3, 1);
}
