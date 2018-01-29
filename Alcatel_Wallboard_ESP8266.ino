#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Soft-AP wifi settings
IPAddress local_IP(10,10,10,1);
IPAddress gateway(10,10,10,254);
IPAddress subnet(255,255,255,0);
const char *ssid = "UDP_to_SERIAL";
const char *password = "12345678"; // Must be 8 chars min (wpa2) see https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/soft-access-point-class.html#softap

// UDP settings
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // Local port to listen on
char incomingPacket[255];  // Buffer for incoming packets

void setup()
{
  // UART0 for debugging
  Serial.begin(115200);

  // UART1 for Alcatel Wallboard (TX pin is GPIO2)
  Serial1.begin(4800, SERIAL_8N2); // Set to 4800 bauds, 8 data bits, No parity, 2 stop bits

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  Serial.printf("   Ssid = %s\n",ssid);
  Serial.print("   Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.print("   Gateway adress = ");
  Serial.println(gateway);
  Serial.print("   Subnet mask = ");
  Serial.println(subnet);
  
  Serial.print("Setting UDP listener ... ");
  Serial.println(Udp.begin(localUdpPort) ? "Ready" : "Failed!");
  Serial.printf("   UDP port = %d\n",localUdpPort);
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // Receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len> 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    Serial1.printf(incomingPacket);
  }
}
