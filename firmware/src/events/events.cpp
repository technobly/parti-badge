#include "Particle.h"

unsigned long lastSentTowerEvent = 0;
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
    // Mesh.publish("tower-send", NULL);
    Particle.publish("tower-send", NULL, PRIVATE);
  }
}
