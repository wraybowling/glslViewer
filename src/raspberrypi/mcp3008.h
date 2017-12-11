#pragma once

#include <wiringPi.h>
#include <wiringPiSPI.h>

void initMCP();
float readMCP(int channel);
float readMCPBasic(int channel);
