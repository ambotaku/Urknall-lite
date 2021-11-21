#ifndef _HABITAT_H_
#define _HABITAT_H_

#include "GFX.hpp"

class Habitat : public GFX {
public:
    /**
     * @brief create lifegame habitat (playfield)
     *
     * @param devAddr device i2c address.
     * @param size oled display size (W128xH64 or W128xH32)
     * @param i2c i2c instance
     */
    Habitat(uint16_t const devAddr, Size size, i2c_inst_t * i2c);

    /*
     * @brief fill display with random pixels
     * @param seed seed for random generator
     * @param density divisor limiting start pixel count
     */
    void randomFill(uint seed=12345, uint density=32);

    /**
     * @brief build next habitat generation
     */
    void nextGen();

#ifdef EXPORT_GENERATION
#include "base64.hpp"
    /*
     * export last rendered buffer as Base64 string
     */
    std::basic_string<unsigned char> exportGeneration() {
      return base64::encode(readBuffer, bufferSize());
    }
#endif

private:
    // return buffersize needed
    uint32_t bufferSize() {
        return width * height / 8;
    }

    // copy of current habitat for nondestructive analysis
	unsigned char * readBuffer;
    uint generation; // generation counter

    /*
     * @brief check pixel's neighborhood (2-3 pixels needed to survive)
     * param x horizontal pixel position
     * param y vertical pixel position
     * @return true if neighbor pixel is not set
     */
    bool isEmptyPixel(int16_t x, int16_t y);

    /*
     * @brief align neighbor's horizontal offset (wrapping around)
     * pos horizontal position of test pixel
     * offset -1 for left, +1 for right neighbor
     * @return aligned x
     */
    int8_t offsetWidth(int8_t pos, int8_t offset);
    
    /*
     * @brief calculate neighbor's vertical offset 
     * pos vertical position of test pixel
     * offset -1 for upper, +1 for lower neighbor
     * @return aligned y
     */
    int8_t offsetHeight(int8_t pos, int8_t offset);

    /*
     * @brief check pixel's complete neighborhood
     * @param x horizontal pixel position
     * @param y vertical pixel position
     * @return neighbor count
     */
    uint8_t checkCell(uint8_t x, uint8_t y);
};

#endif
