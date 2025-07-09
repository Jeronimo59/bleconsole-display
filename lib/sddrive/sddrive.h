#ifndef SDDRIVE_H_
#define SDDRIVE_H_

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <lvgl.h>

class SDDrive
{
public:
   SDDrive(SPIClass *spi) : spi(spi) {}

   bool begin(char letter, uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t cs);

   void end()
   {
      spi->end();
   }

   SPIClass *getSPI()
   {
      return spi;
   }

   int getCSPin()
   {
      return csPin;
   }

protected:
   bool isInitialized = false;
   SPIClass *spi;
   int csPin;
};

#endif