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

#define CS 0
#define MOSI 12
#define CLOCK 14

#define START digitalWrite(CS, HIGH)
#define END digitalWrite(CS, LOW)

unsigned char buffer[128*64/8];

void peak() {
    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(1);
    digitalWrite(CLOCK, LOW);
    delayMicroseconds(1);
}

void write(unsigned char data) {
	for (int i=0;i<8;i++) {
		digitalWrite(MOSI, data & 0b10000000);
		peak();
		data = data << 1;
	}
}

void writeword(unsigned char data) {
	write(data & 0b11110000);
	write(data << 4);
}

void lcdInit() {
	pinMode (CS, OUTPUT) ;
	pinMode (MOSI, OUTPUT) ;
	pinMode (CLOCK, OUTPUT) ;

	START;
	write(0b11111000);
	writeword(0b00000001);
    delay(2000);
	writeword(0b00001100);
    delay(1);
	writeword(0b00110100);
    delay(1);
	writeword(0b00110110);
    delay(1);
//	writeword(0b00000011);
//    delay(1);
	writeword(0b10000000);
	writeword(0b10000000);
    delay(1);
	END;
}

void toggleBuffer() {
	for (unsigned char line = 0; line <32; line ++) {
		START;
		write(0b11111000);
		writeword(0b10000000 + line);
		writeword(0b10000000);
		END;

		START;
		write(0b11111010);
		for (unsigned short col = 0; col < 16; col ++) {
			writeword(0b10011001);
			//writeword(buffer[line*32+col]);
		}
		for (unsigned short col = 0; col < 16; col ++) {
			writeword(0b01100110);
			//writeword(buffer[(line+32)*32+col]);
		}
		END;
	}
}


int main(int argc, char **argv) {
	wiringPiSetup () ;

	for (int i=0;i<128*64/8;i++) 
		buffer[i] = 0b11111111;
		

	lcdInit();
	delay(10);
	toggleBuffer();
	

	return 0;
}

