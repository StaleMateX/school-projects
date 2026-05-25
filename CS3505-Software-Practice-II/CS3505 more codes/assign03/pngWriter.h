#ifndef PNGWRITER_H
#define PNGWRITER_H

#include <iostream>
#include <png.h>

/**
 * that manages the data necessary for libpng to work and that provides class methods 
 * to draw individual pixels to an image and write that image to a png file. 
 * The .h file should only provide the interface and documentation and 
 * all implementation should be done in pngWriter.cpp.
 * 
 * by Jacob Xu
*/

class PNGWriter{
  private:
        int width;
        int height;
        // Each pixel requires 4 bytes for [red, green, blue, alpha]
        unsigned char* imageData; // unsigned char is equivalent to png_byte
        png_bytep* imageRows; // png_bytep is a pointer type, so png_bytep* is a pointer to a pointer


  public:
        //Constructor
        PNGWriter(int width, int height);

        //Destructor
        ~PNGWriter();

        // Draw a pixel to an image data array at a specific x and y pixel location. 
        void drawPixelImage(int x, int y, int red, int green, int blue, int alpha);

        //Write the contents of the image data array to a png file.
        void writePNGFile(char *filename);

};

#endif