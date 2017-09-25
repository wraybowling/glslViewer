#include <wiringPi.h>
#include <wiringPiSPI.h>

// GET
//----------------------------------------------
int mcp3008Setup();
float mcp3008Read(int channel);
