/*
 * SpiSt7920.cxx
 * 
 * Copyright 2017  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CHANNEL 0

void sendCommand(unsigned char command) {
	unsigned char buffer[3];
	buffer[0] = 0b11111000;
	buffer[1] = command & 0b11110000;
	buffer[2] = command << 4;
	wiringPiSPIDataRW(CHANNEL, buffer, 3);
}


int main(int argc, char **argv)
{
    std::cout << "init" << wiringPiSPISetup(CHANNEL, 50000) << "\n";
	
	std::cout << "clear\n";
	sendCommand(0b00000001);
    delay(2000);
	std::cout << "on\n";
	sendCommand(0b00001100);
    delay(2000);
	std::cout << "ext 8 \n";
	sendCommand(0b00110100);
    delay(2000);
	std::cout << "ext 8 gr\n";
	sendCommand(0b00110110);
    delay(2000);
	
	return 0;
}

