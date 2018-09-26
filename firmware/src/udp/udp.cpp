#include "application.h"
#include "ifapi.h"
#include "random.h"

static UDP udp;
static Random rng;
static IPAddress mcastAddr;
static const uint16_t PORT = 10000;
static const system_tick_t PERIOD = 1000;
static const char MULTICAST_ADDR[] = "ff03::1:1001";
static const size_t IP_ADDR_STRLEN_MAX = IP6ADDR_STRLEN_MAX;

static uint8_t buf[1000] = {};

void openSocket()
{
  Mesh.connect();
  udp.stop();

  // Get OpenThread interface index (OpenThread interface is named "th1" on all Mesh devices)
  uint8_t idx = 0;
  if_name_to_index("th1", &idx);

  Serial.printlnf("th1 index = %u", idx);

  // Create UDP socket and bind to OpenThread interface
  auto ur = udp.begin(PORT, idx);
  Serial.printlnf("UDP.begin() = %u", ur);

  // Subscribe to ff03::1:1001
  HAL_IPAddress addr = {};
  addr.v = 6;
  inet_inet_pton(AF_INET6, MULTICAST_ADDR, addr.ipv6);
  mcastAddr = addr;
  int r = udp.joinMulticast(mcastAddr);
  Serial.printlnf("Subscribed to %s: %d", MULTICAST_ADDR, r);
}

void sendMessage(String message)
{
  static system_tick_t last = 0;

  if ((millis() - last) >= PERIOD)
  {
    size_t len = random(1, sizeof(buf));
    rng.gen((char *)buf, len);
    auto r = udp.sendPacket(buf, len, mcastAddr, PORT);
    Serial.printlnf("Sent %u bytes to %s#%u : %d %d", len, MULTICAST_ADDR, PORT, r, errno);
    last = millis();
    if (r < 0)
    {
      openSocket();
    }
  }
}

String receiveMessage()
{
  ssize_t s = udp.receivePacket(buf, sizeof(buf));
  if (s > 0)
  {
    char tmp[IP_ADDR_STRLEN_MAX] = {};
    auto ip = udp.remoteIP();
    Serial.printlnf("Received %d bytes from %s#%u", s,
                    inet_inet_ntop(AF_INET6, ip.raw().ipv6, tmp, sizeof(tmp)), udp.remotePort());
  }

  return String("Hello");
}