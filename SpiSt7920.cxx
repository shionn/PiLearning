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
//#include <wiringPiSPI.h>
#include <bcm2835.h>


#define CHANNEL 0
#define CS 0
#define MOSI 12
#define CLOCK 14

#define START digitalWrite(CS, HIGH)
#define END digitalWrite(CS, LOW)

void sendCommand(char command) {
	/*char buffer[3];
	buffer[0] = 0b11111000;
	buffer[1] = command & 0b11110000;
	buffer[2] = command << 4;*/
    START;
    bcm2835_spi_transfer(0b11111000);
    bcm2835_spi_transfer(command & 0b11110000);
    bcm2835_spi_transfer(command << 4);
    
	//bcm2835_spi_writenb(buffer, 3);
    END;
}

void locate() {
	START;
    bcm2835_spi_transfer(0b11111000);
	bcm2835_spi_transfer(0b10000000);
	bcm2835_spi_transfer(0);
	bcm2835_spi_transfer(0b10000000);
	bcm2835_spi_transfer(0);
	END;
}

void sendData(char data) {
	/*char buffer[3];
	buffer[0] = 0b11111010;
	buffer[1] = data & 0b11110000;
	buffer[2] = data << 4;*/
    START;
    bcm2835_spi_transfer(0b11111010);
    bcm2835_spi_transfer(data & 0b11110000);
    bcm2835_spi_transfer(data << 4);
    
	//bcm2835_spi_writenb(buffer, 3);
    END;
}


int main(int argc, char **argv) {
	std::cout << wiringPiSetup () << "\n";
	pinMode (CS, OUTPUT) ;
	std::cout << bcm2835_init() << "\n";
	
	std::cout <<  bcm2835_spi_begin() << "\n";
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, HIGH);      // the default
	
	
	sendCommand(0b00000001);
    delay(2000);
	sendCommand(0b00001100);
    delay(1);
	sendCommand(0b00110100);
    delay(1);
	sendCommand(0b00110110);
    delay(1);
    
    locate();
    delay(1);
    sendData(0xFF);
    sendData(0b00111100);
    sendData(0b11001100);
    
    
    
    bcm2835_spi_end();
    bcm2835_close();
	
	return 0;
}

