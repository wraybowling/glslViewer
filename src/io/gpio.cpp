#include "gpio.h"

#include <iostream>
//#include <unistd.h>

//#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <mcp3004.h>

#define CHANNEL_CONFIG_SINGLE_ENDED 8

#define BASE 64
#define SPI_CHANNEL 0
#define CLOCK_SPEED 500000

namespace gpio{
  void startMCP3004() {
    std::cout << "Started WiringPi on SPI " << SPI_CHANNEL << std::endl;
    wiringPiSPISetup(SPI_CHANNEL, CLOCK_SPEED);
    mcp3004Setup(BASE, SPI_CHANNEL);
  }

  float readMcpSpi(int channel) {
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (CHANNEL_CONFIG_SINGLE_ENDED + channel) << 4;
    wiringPiSPIDataRW(channel, buffer, 3);
    unsigned int result = ( (buffer[1] & 3) << 8 ) + buffer[2]; // get last 10 bits
    return result / 1023.0;
  }
}

