void fireNamedEvent(const char *deviceId) {
  Particle.publish("tc-named", deviceId);
}
