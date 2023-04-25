#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

class Matrix
{
private:
    /* data */
    uint8_t w, h, pin;
public:
    Adafruit_NeoMatrix *matrix;

    Matrix(uint8_t w, uint8_t h, uint8_t pin);
};
