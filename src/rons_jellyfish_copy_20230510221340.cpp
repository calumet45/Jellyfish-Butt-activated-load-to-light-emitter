/**
 *
 * HX711 library for Arduino - example file
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula


 NudiNet
 plankt0n
 *
**/
#include "HX711.h"
#include "WiFi.h"
// #include "AsyncUDP.h"
#include "WiFiUdp.h"
#include "Udp.h"

// AsyncUDP udp;
WiFiUDP udp;
const char *ssid = "NudiNet";
const char *password = "plankt0n";
boolean connected = false;
const int portMulti = 2222;
// const char * ipMulti = "239,0,0,1";

// void setup()
// {
//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   if (WiFi.waitForConnectResult() != WL_CONNECTED)
//   {
//     Serial.println("WiFi Failed");
//     while (1)
//     {
//       delay(1000);
//     }
//   }
//   if (udp.listen(1234))
//   {
//     Serial.print("UDP Listening on IP: ");
//     Serial.println(WiFi.localIP());
//     udp.onPacket([](AsyncUDPPacket packet)
//                  {
//             Serial.print("UDP Packet Type: ");
//             Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
//             Serial.print(", From: ");
//             Serial.print(packet.remoteIP());
//             Serial.print(":");
//             Serial.print(packet.remotePort());
//             Serial.print(", To: ");
//             Serial.print(packet.localIP());
//             Serial.print(":");
//             Serial.print(packet.localPort());
//             Serial.print(", Length: ");
//             Serial.print(packet.length());
//             Serial.print(", Data: ");
//             Serial.write(packet.data(), packet.length());
//             Serial.println();
//             //reply to the client
//             packet.printf("Got %u bytes of data", packet.length()); });
//   }
// }

// void loop()
// {
//   delay(1000);
//   // Send broadcast
//   udp.broadcast("Anyone here?");
// }
// void setup()
// {
//   Serial.begin(115200);
//   connectToWiFi(ssid, password);
// }

// void loop()
// {
//   if (connected) {
//     int len = udp.parsePacket();
//     if (len > 0) {
//       Serial.print("src IP: ");
//       Serial.print(udp.remoteIP());
//       Serial.print(";    packet: [");
//       char buf[len];
//       udp.read(buf, len);
//       for (int i = 0; i < len; i++) {
//         Serial.print((char)buf[i]);
//       }
//       //String str(buf);
//       Serial.println("]");
//     }
//   }
// }

// //wifi event handler
void WiFiEvent(WiFiEvent_t event)
{
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    // When connected set
    Serial.print("WiFi connected! IP address: ");
    Serial.println(WiFi.localIP());
    // udp.broadcast()
    // udp.beginPacket('192.168.1.255', 2222); // libmapper admin bus multicast
    // udp.begin(7000); //test regular UDP server endpoint
    connected = true;
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    Serial.println("WiFi lost connection");
    connected = false;
    break;
  }

  WiFi.mode(WIFI_STA);
  //   WiFi.begin(ssid, password);
  //   if (WiFi.waitForConnectResult() != WL_CONNECTED)
  //   {
  //     Serial.println("WiFi Failed");
  //     while (1)
  //     {
  //       delay(1000);
  //     }
  //   }
}
void connectToWiFi(const char *ssid, const char *pwd)
{
  Serial.println("Connecting to WiFi network: " + String(ssid));
  // delete old config
  WiFi.disconnect(true);
  // register event handler
  WiFi.onEvent(WiFiEvent);
  // Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;

HX711 scale;

void setup()
{
  Serial.begin(115200);
  connectToWiFi(ssid, password);

  // Serial.println("HX711 Demo");

  // Serial.println("Initializing the scale");

  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read()); // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight (not set) divided
                                         // by the SCALE parameter (not set yet)

  scale.set_scale(2280.f); // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();            // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read()); // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5)); // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1); // print the average of 5 readings from the ADC minus tare weight, divided
                                         // by the SCALE parameter set with set_scale

  Serial.println("Readings:");

  // udp.beginMulticast('239.0.0.1', 2222);
}

void loop()
{
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 1);

  if (connected)
  {

    // udp.beginMulticastPacket();
    udp.beginPacket(IPAddress(192, 168, 1, 255), 2222); // libmapper admin bus multicast
    byte tmp = scale.get_units(10);
    udp.write(tmp);
    udp.endPacket();
  }

  scale.power_down(); // put the ADC in sleep mode
  delay(1000);
  scale.power_up();
}
