#include "application.h"
#include "display/display.h"
#include "leds/leds.h"

//MeshSignal signal;

void meshSetup()
{
  Mesh.on();
  Mesh.connect();
}

void towerFullHandler(const char *event, const char *data)
{
  ledChase();
}

void towerFullSloMohandler(const char *event, const char *data)
{
  // TODO: Replace with slow-mo replay
  ledPulseChase();
}

void meshPingHandler(const char *event, const char *data)
{
  displayMeshPing();
}

void zachButtonHandler(const char *event, const char *data)
{
  // TODO: Replace with display text and LED animation
  // displayHiFromZ();
  ledRandom();
}

void gatewayPublishHandler(const char *event, const char *data)
{
  displayGatewayPing(data);
}

void setupMeshEvents()
{
  Mesh.subscribe("tower-full", towerFullHandler);
  Mesh.subscribe("tower-full-slomo", towerFullSloMohandler);
  Mesh.subscribe("mesh-ping", meshPingHandler);
  Mesh.subscribe("zach-button", zachButtonHandler);

  // Workshop subscriptions
  Mesh.subscribe("gateway-setup-clicked", gatewayPublishHandler);
}

void getMeshNetworkInfo()
{
  Serial.printlnf("Connecting? %s", Mesh.connecting() ? "YES" : "NO");
  Serial.printlnf("Network ready? %s", Mesh.ready() ? "YES" : "NO");
  Serial.printlnf("Listening? %s", Mesh.listening() ? "YES" : "NO");
  //Serial.printlnf("Signal: %f", signal.getStrength());
}