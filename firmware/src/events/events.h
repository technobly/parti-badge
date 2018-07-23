void fireNamedEvent(const char *deviceId) {
  Particle.publish("tc-named", deviceId);
}

void fireIREvent(const char *deviceId) {
  Particle.publish("tc-blastem", deviceId);
}

void fireSimonEvent(const char *deviceId) {
  Particle.publish("tc-simon-player", deviceId);
}

void fireSimonWinnerEvent(const char *deviceId) {
  Particle.publish("tc-simon-winner", deviceId);
}

void fireRickEvent(const char *deviceId) {
  Particle.publish("tc-simon-player", deviceId);
}

void fireKonamiEvent(const char *deviceId) {
  Particle.publish("tc-konami", deviceId);
}

void fireMEPEvent(const char *deviceId) {
  Particle.publish("tc-mep", deviceId);
}

void fireBRBEvent(const char *deviceId) {
  // The Big Red Button event
  // You need to find the name of this event in order to publish.
  Particle.publish("???", deviceId);

  //Want a hint?
  //What's the Big Red Button? And is there a map to it somewhere?
}

void fireSecret(const char *deviceId) {
  // A SECRET EVENT!
  // You need to find the name of this event in order to publish.
  Particle.publish("?????", deviceId);

  //Want a hint?
  //1. Find the Spark...
}
