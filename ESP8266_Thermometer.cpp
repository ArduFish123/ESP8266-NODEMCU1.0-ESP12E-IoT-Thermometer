/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/
#include "Arduino.h"
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID   "Template ID"
#define BLYNK_DEVICE_NAME "Device name"
#define BLYNK_FIRMWARE_VERSION "x.x.x"
#define dht_apin 5 // GPIO Pin sensor is connected too (GPIO 5 is D1 on NodeMCU 1.0 ESP12E)
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266_SSL.h>
#include <dht.h>

dht DHT;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Auth Token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wifi name";
char pass[] = "WiFi pass";

BLYNK_WRITE(InternalPinOTA) {
  String url = param.asString();
  Blynk.disconnect();
  // Procceed to download and apply the upgrade
}

void resetMCU()
{
#if defined(ARDUINO_ARCH_MEGAAVR)
  wdt_enable(WDT_PERIOD_8CLK_gc);
#elif defined(__AVR__)
  wdt_enable(WDTO_15MS);
#elif defined(__arm__)
  NVIC_SystemReset();
#elif defined(ESP8266) || defined(ESP32)
  ESP.restart();
#else
  #error "MCU reset procedure not implemented"
#endif
  for (;;) {}
}

BLYNK_WRITE(InternalPinDBG) {
  if (String(param.asStr()) == "reboot") {
    Serial.println("Rebooting...");

    // TODO: Perform any neccessary preparation here,
    // i.e. turn off peripherals, write state to EEPROM, etc.

    // NOTE: You may need to defer a reboot,
    // if device is in process of some critical operation.

    resetMCU();
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  sendBlynkData();
}

void loop()
{
  Blynk.run();
}

void sendBlynkData()
{
while (true){
  DHT.read11(dht_apin);
  Blynk.virtualWrite(V0, DHT.temperature);
  Blynk.virtualWrite(V1, DHT.humidity);
  Blynk.virtualWrite(V2, DHT.temperature - (100 - DHT.humidity) / 5);
  delay(5000);
  }
}
