#include "Adafruit_Si7021.h"
#include "display/display.h"
#include "events/events.h"

extern Adafruit_Si7021 envSensor;
extern int currentTemp;
extern int currentHumidity;
extern int currentBatteryReading;
extern bool displayingTemp;
extern bool displayingBattery;

// Get temp and humidity from the sensors
void getTempAndHumidity()
{
  int prevTemp = currentTemp;
  int prevHumidity = currentHumidity;

  currentTemp = round((envSensor.readTemperature() * 1.8 + 32.00) * 10) / 10;
  currentHumidity = round(envSensor.readHumidity() * 10) / 10;

  fireEnvSensorsEvent(currentTemp, currentHumidity);

  // If either has changed and these values are being displayed, update the display
  if (displayingTemp && (prevTemp != currentTemp || prevHumidity != currentHumidity))
  {
    showTempAndHumidity();
  }
}

void getBatteryReading()
{
  int prevBattery = currentBatteryReading;

  currentBatteryReading = round(analogRead(BATT) * 100 / 4095);

  fireBatteryEvent(currentBatteryReading);

  if (displayingBattery && (prevBattery != currentBatteryReading))
  {
    showBattery();
  }
}