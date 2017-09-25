/*
 * readMCP3008.c
 * read values from an MPC3008 Analog to Digital Converter
 * 
 * from shaunsbennett.com/piblog/?p=266
 *
 * compile with the following command
 *
 * gcc -Wall -o readMCP3008 readMPC3008.c -lwiringPi
 *
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <mcp3004.h>

#define CHANNEL_CONFIG_SINGLE_ENDED 8

#define BASE 64
#define SPI_CHANNEL 0
#define CLOCK_SPEED 500000

float getAnalog(int channel)
{
  unsigned char buffer[3] = {1}; // start bit
  buffer[1] = (CHANNEL_CONFIG_SINGLE_ENDED + channel) << 4;
  wiringPiSPIDataRW(channel, buffer, 3);
  unsigned int result = ( (buffer[1] & 3) << 8 ) + buffer[2]; // get last 10 bits
  return result / 1024.0;
}

