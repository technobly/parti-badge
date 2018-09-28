#include "Particle.h"
#include "qMenuSystem.h"

extern qMenuSystem menu;

unsigned long lastSentTowerEvent = 0;
unsigned long lastHelloEvent = 0;
unsigned long lastSentZEvent = 0;
unsigned long eventDelay = 5000;

void fireEnvSensorsEvent(int temp, int humidity)
{
  Particle.publish("env-sensors", "{\"temp\":" + String(temp) + ",\"humidity\":" + String(humidity) + "}", PRIVATE);
}

void fireBatteryEvent(int battery)
{
  Particle.publish("battery-charge", String(battery), PRIVATE);
}

void fireTowerSendEvent()
{
  // Make sure that events are at least five seconds apart
  if (lastSentTowerEvent == 0 || millis() - lastSentTowerEvent > eventDelay)
  {
    lastSentTowerEvent = millis();
    Mesh.publish("tower-send", NULL);
  }
}

// TODO: Re-implement a call to this for Zach's badge
void fireHelloFromZEvent()
{
  // Make sure that events are at least five seconds apart
  if (lastSentZEvent == 0 || millis() - lastSentZEvent > eventDelay)
  {
    lastSentZEvent = millis();
    Mesh.publish("zach-button", NULL);
  }
}

void fireHelloPingEvent()
{
  // Make sure that events are at least five seconds apart
  if (lastHelloEvent == 0 || millis() - lastHelloEvent > eventDelay)
  {
    lastHelloEvent = millis();
    Mesh.publish("mesh-ping", "hello!");

    menu.MessageBox("Mesh ping sent!");
  }
}
