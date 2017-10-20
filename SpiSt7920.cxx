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
#define CS 0
#define MOSI 12
#define CLOCK 14



void sendCommand(unsigned char command) {
	unsigned char buffer[3];
	buffer[0] = 0b11111000;
	buffer[1] = command & 0b11110000;
	buffer[2] = command << 4;
    digitalWrite(CS, HIGH);
	wiringPiSPIDataRW(CHANNEL, buffer, 3);
    digitalWrite(CS, LOW);
	
}

void locate(unsigned char x, unsigned char y) {
	unsigned char buffer[5];
	buffer[0] =   0b11111000;
	buffer[1] = ( 0b10000000 + y ) & 0b11110000;
	buffer[2] = ( 0b10000000 + y ) << 4;
	buffer[3] = ( 0b10000000 + x ) & 0b11110000;
	buffer[4] = ( 0b10000000 + x ) << 4;
	
    digitalWrite(CS, HIGH);
	wiringPiSPIDataRW(CHANNEL, buffer, 5);
	digitalWrite(CS, LOW);

}

void draw() {
	unsigned char pattern = 0b11100111;
    unsigned char buffer[3];
	buffer[0] = 0b11111010;
	
	digitalWrite(CS, HIGH);
	wiringPiSPIDataRW(CHANNEL, buffer, 1);
	buffer[1] = pattern & 0b11110000;
	buffer[2] = pattern << 4;
	for (int y = 0; y< 16;y++) {
		for (int x= 0; x<32;x++) {
			wiringPiSPIDataRW(CHANNEL, buffer, 2);
			delay(1);
		}
	}
	digitalWrite(CS, LOW);
}

int main(int argc, char **argv) {
	wiringPiSetup () ;
	pinMode (CS, OUTPUT) ;

    std::cout << "init" << wiringPiSPISetup(CHANNEL, 100000) << "\n";
    
	
	sendCommand(0b00000001);
    delay(2000);
	sendCommand(0b00001100);
    delay(100);
	sendCommand(0b00110100);
    delay(100);
	sendCommand(0b00110110);
    delay(100);
    
    locate(0,0);
    delay(500);
    
	draw();
	
		digitalWrite(CS, HIGH);

	return 0;
}

