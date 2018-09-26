#include "application.h"

//MeshSignal signal;

void getMeshNetworkInfo()
{
  Serial.printlnf("Connecting? %s", Mesh.connecting() ? "YES" : "NO");
  Serial.printlnf("Network ready? %s", Mesh.ready() ? "YES" : "NO");
  Serial.printlnf("Listening? %s", Mesh.listening() ? "YES" : "NO");
  //Serial.printlnf("Signal: %f", signal.getStrength());
}