#include <Arduino.h>

#include "tlc59711.h"

#define NUM_TLC59711 8

TLC59711 tlc = TLC59711(NUM_TLC59711);

void setup()
{
  Serial.begin(9600);
  Serial.println("setup()");
  tlc.begin(SCK, MOSI);
  // tlc.beginSPI();
}

void loop()
{
  for (int i = 0; i < 4 * NUM_TLC59711; i++)
  {
    tlc.resetLeds();
    tlc.setLED(i, 65535, 65535, 65535);
    tlc.write();
  }
}
