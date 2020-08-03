#pragma once

//#include <wiringPi.h>
#include <wiringPiSPI.h>

namespace gpio{

void startMCP3004();
float readMcpSpi(int channel);
float readMcpBasic(int channel);

}
