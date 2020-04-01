#if !defined(TLC59711_H)
#define TLC59711_H

#include <Arduino.h>
#include <SPI.h>

class TLC59711
{
private:
    uint16_t *pwmbuffer;
    uint32_t command;
    uint8_t BCr, BCg, BCb, numdrivers, _c, _d;
    SPIClass *_spi;

public:
    TLC59711(uint8_t n);
    ~TLC59711();

    void begin(uint8_t c, uint8_t d);
    void beginSPI();
    void setPWM(uint8_t chan, uint16_t pwm);
    void setLED(uint8_t lednum, uint16_t r, uint16_t g, uint16_t b);
    void write();
    void writeSPI();
    void setBrightness(uint8_t bcr, uint8_t bcg, uint8_t bcb);
    void simpleSetBrightness(uint8_t BC);
    void resetLeds();
};

#endif // TLC59711_H
