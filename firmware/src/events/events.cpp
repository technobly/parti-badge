#include "Particle.h"

void fireEnvSensorsEvent(int temp, int humidity)
{
  Particle.publish("env-sensors", "{\"temp\":" + String(temp) + ",\"humidity\":" + String(humidity) + "}", PRIVATE);
}

void fireBatteryEvent(int battery)
{
  Particle.publish("battery-charge", String(battery), PRIVATE);
}
