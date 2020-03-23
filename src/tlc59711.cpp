#include "tlc59711.h"

SPISettings SPI_SETTINGS(2000000, MSBFIRST, SPI_MODE0);

TLC59711::TLC59711(uint8_t n)
{
  numdrivers = n;
  simpleSetBrightness(127);
  pwmbuffer = (uint16_t *)calloc(2, 12 * n);
}

TLC59711::~TLC59711()
{
}

void TLC59711::beginSPI()
{
  _spi = new SPIClass(VSPI);
  _spi->begin();
}

void TLC59711::begin(uint8_t c, uint8_t d)
{
  _c = c;
  _d = d;
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
}

void TLC59711::setPWM(uint8_t chan, uint16_t pwm)
{
  if (chan > 12 * numdrivers)
    return;
  pwmbuffer[chan] = pwm;
}

void TLC59711::resetLeds()
{
  for (uint8_t n = 0; n < 12*numdrivers; n++)
  {
    pwmbuffer[n] = 0;
  }
}

void TLC59711::setLED(uint8_t lednum, uint16_t r, uint16_t g,
                      uint16_t b)
{
  setPWM(lednum * 3, r);
  setPWM(lednum * 3 + 1, g);
  setPWM(lednum * 3 + 2, b);
}

void TLC59711::simpleSetBrightness(uint8_t bc)
{
  setBrightness(bc, bc, bc);
}

void TLC59711::setBrightness(uint8_t bcr, uint8_t bcg, uint8_t bcb)
{
  if (bcr > 127)
  {
    bcr = 127; // maximum possible value since BC can only be 7 bit
  }
  else if (bcr < 0)
  {
    bcr = 0;
  }

  BCr = bcr;

  if (bcg > 127)
  {
    bcg = 127; // maximum possible value since BC can only be 7 bit
  }
  else if (bcg < 0)
  {
    bcg = 0;
  }

  BCg = bcg;

  if (bcb > 127)
  {
    bcb = 127; // maximum possible value since BC can only be 7 bit
  }
  else if (bcb < 0)
  {
    bcb = 0;
  }

  BCb = bcb;

  // Write command
  command = 0x25;

  command <<= 5;
  // OUTTMG = 1, EXTGCK = 0, TMGRST = 0, DSPRPT = 1, BLANK = 0 -> 0x12
  command |= 0x12;

  command <<= 7;
  command |= BCb;

  command <<= 7;
  command |= BCg;

  command <<= 7;
  command |= BCr;

}

void TLC59711::write()
{
  noInterrupts();
  for (uint8_t n = 0; n < numdrivers; n++)
  {
    shiftOut(_d, _c, MSBFIRST, command >> 24);
    shiftOut(_d, _c, MSBFIRST, command >> 16);
    shiftOut(_d, _c, MSBFIRST, command >> 8);
    shiftOut(_d, _c, MSBFIRST, command);

    // 12 channels per TLC59711
    for (int8_t c = 11; c >= 0; c--)
    {
      // 16 bits per channel, send MSB first
      shiftOut(_d, _c, MSBFIRST, pwmbuffer[n * 12 + c] >> 8);
      shiftOut(_d, _c, MSBFIRST, pwmbuffer[n * 12 + c]);
    }
  }
  delayMicroseconds(4);
  interrupts();
}

void TLC59711::writeSPI()
{
  _spi->beginTransaction(SPI_SETTINGS);

  noInterrupts();
  for (uint8_t n = 0; n < numdrivers; n++)
  {
    _spi->transfer(command >> 24);
    _spi->transfer(command >> 16);
    _spi->transfer(command >> 8);
    _spi->transfer(command);

    // 12 channels per TLC59711
    for (int8_t c = 11; c >= 0; c--)
    {
      // 16 bits per channel, send MSB first
      _spi->transfer(pwmbuffer[n * 12 + c] >> 8);
      _spi->transfer(pwmbuffer[n * 12 + c]);
    }
  }

  delayMicroseconds(4);
  _spi->endTransaction();

  interrupts();
}
