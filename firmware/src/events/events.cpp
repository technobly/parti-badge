#include "Particle.h"
#include "qMenuSystem.h"

extern qMenuSystem menu;

unsigned long lastSentTowerEvent = 0;
unsigned long lastHelloEvent = 0;
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

void fireHelloPingEvent()
{
  // Make sure that events are at least five seconds apart
  if (lastHelloEvent == 0 || millis() - lastHelloEvent > eventDelay)
  {
    lastHelloEvent = millis();
    Mesh.publish("mesh-ping", "hello!");

    menu.MessageBox("You sent a ping!");
  }
}
