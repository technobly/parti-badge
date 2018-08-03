void fireNamedEvent() {
  Particle.publish("tc-hunt-event", "tc-named", PRIVATE);
}

void fireIREvent(String code) {
  Particle.publish("tc-hunt-event", "tc-blastem-" + code, PRIVATE);
}

void fireSimonEvent() {
  Particle.publish("tc-hunt-event", "tc-simon-player", PRIVATE);
}

void fireSimonWinnerEvent() {
  Particle.publish("tc-hunt-event", "tc-simon-winner", PRIVATE);
}

void fireRickEvent() {
  Particle.publish("tc-hunt-event", "tc-rick", PRIVATE);
}

void fireKonamiEvent() {
  Particle.publish("tc-hunt-event", "tc-konami", PRIVATE);
}

void fireEnvSensorsEvent(int temp, int humidity) {
  Particle.publish("tc-env-sensors", "{\"temp\":" + String(temp) + ",\"humidity\":" + String(humidity) + "}", PRIVATE);
}

void fireBRBEvent() {
  // The Big Red Button event
  // You need to find the name of this event in order to publish.
  Particle.publish("tc-hunt-event", "???", PRIVATE);

  //Want a hint?
  //What's the Big Red Button? And is there a map to it somewhere?
}

void fireSecret() {
  // A SECRET EVENT!
  // You need to find the name of this event in order to publish.
  Particle.publish("tc-hunt-event", "?????", PRIVATE);

  //Want a hint?
  //1. Find the Spark...
}
