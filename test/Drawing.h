#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

class Drawing
{
private:
    Adafruit_NeoMatrix* matrix;
public:
    Drawing(Adafruit_NeoMatrix* matrix){this->matrix=matrix;}
    void drawHex(uint8_t* icon,uint16_t color){
        bool Bitmap[256];
        uint8_t j=3;
        for(;icon;icon++,j+=7){
            uint8_t h=*icon;
            while (h)
            {
                Bitmap[j]=h%2;
                h/=2;
                j--;
            }
            
        }
        //matrix->drawBitmap(0,0,&Bitmap,32,8,color);
    }

};
