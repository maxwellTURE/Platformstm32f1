/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include "ESP8266/ESP8266.h"

#define SSID        "234"
#define PASSWORD    "0987654321"
#define HOST_NAME   "192.168.43.22"
#define HOST_PORT   (8090)
#define LED0 PB5
//                     RX    TX
HardwareSerial Serial1(PA10, PA9);
HardwareSerial Serial3(PB11, PB10);

ESP8266 wifi(Serial3, 115200);

void setup()
{
    Serial1.begin(115200);
    Serial1.print("setup begin\r\n");

    Serial1.print("FW Version:");
    Serial1.println(wifi.getVersion().c_str());

    if (wifi.setOprToStationSoftAP()) {
        Serial1.print("to station + softap ok\r\n");
    } else {
        Serial1.print("to station + softap err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial1.print("Join AP success\r\n");
        Serial1.print("IP:");
        Serial1.println( wifi.getLocalIP().c_str());
    } else {
        Serial1.print("Join AP failure\r\n");
    }

    if (wifi.disableMUX()) {
        Serial1.print("single ok\r\n");
    } else {
        Serial1.print("single err\r\n");
    }

    Serial1.print("setup end\r\n");
}

void loop()
{
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED0, HIGH);
    // wait for a second
    delay(1000);
    // turn the LED off by making the voltage LOW
    digitalWrite(LED0, LOW);
    // wait for a second
    delay(1000);


    uint8_t buffer[128] = {0};

    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial1.print("create tcp ok\r\n");
    } else {
        Serial1.print("create tcp err\r\n");
    }

    char *hello = "Hello, this is client!";
    wifi.send((const uint8_t*)hello, strlen(hello));

    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        Serial1.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial1.print((char)buffer[i]);
        }
        Serial1.print("]\r\n");
    }

    if (wifi.releaseTCP()) {
        Serial1.print("release tcp ok\r\n");
    } else {
        Serial1.print("release tcp err\r\n");
    }
//    delay(5000);
}
