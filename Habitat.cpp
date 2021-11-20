#include <stdio.h>
#include <stdlib.h>
#include "Habitat.hpp"

Habitat::Habitat(uint16_t const devAddr, Size size, i2c_inst_t * i2c) : GFX(devAddr, size, i2c) {
	this->readBuffer = new unsigned char[this->width*this->height/8];
    generation = 0;
};

int8_t Habitat::offsetWidth(int8_t pos, int8_t offset) {
    if (pos + offset < 0)  {
        return width + offset;
    } else if (pos + offset >= width) {
        return pos - width + offset;
    } else {
        return pos + offset;
    }
}

int8_t Habitat::offsetHeight(int8_t pos, int8_t offset) {
    if (pos + offset < 0)  {
        return height + offset;
    } else if (pos + offset >= height) {
        return pos - height + offset;
    } else {
        return pos + offset;
    }
}

bool Habitat::isEmptyPixel(int16_t x, int16_t y) {
	uint8_t byte = this->readBuffer[x + (y/8) * this->width];
    uint8_t n = (1 << (y&7))&byte;
	return n == 0;
}

uint8_t Habitat::checkCell(uint8_t x, uint8_t y) {
    static int offsets[8][2] = {{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};
    uint8_t neighbors = 0;
    for (uint8_t idx=0; idx < 8; ++idx) {
        int *ofs= offsets[idx];
        bool emptyNeighbor = isEmptyPixel(offsetWidth(x, ofs[0]), offsetHeight(y, ofs[1]));
        if (!emptyNeighbor) {
            ++neighbors;
        }
    }
    return neighbors;
}

void Habitat::randomFill(uint seed, uint density) {
    srand(seed);    
    for(uint8_t x=0; x<width; ++x) {
        for(uint8_t y=0; y<height; ++y) {
            drawPixel(x, y, (rand() < RAND_MAX/density) ? Colors::WHITE : Colors::BLACK);
        }
    }
}

void Habitat::nextGen() {
    memcpy(readBuffer, buffer, this->width*this->height/8);
    for(uint8_t x=0; x<width; ++x) {
        for(uint8_t y=0; y<height; ++y) {
            uint8_t neighbors = checkCell(x, y);
            drawPixel(x, y, (neighbors<2 || neighbors>3) ? Colors::BLACK : Colors::WHITE);
        }
    }
    
    printf("generation %d\r\n", ++generation);
}