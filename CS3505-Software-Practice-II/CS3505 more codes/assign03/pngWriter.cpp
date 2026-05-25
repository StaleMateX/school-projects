#include "pngWriter.h"
#include <png.h>

//Constructor
PNGWriter::PNGWriter(int width, int height) : width(width), height(height) {
    // Allocate memory for the image data
    imageData = new unsigned char[width * height * 4]; // Assuming RGBA format
    imageRows = new png_bytep[height];
}
//Destructor
PNGWriter::~PNGWriter() {
    // Deallocate memory
    delete[] imageData;
    delete[] imageRows;
}
// Draw a pixel to an image data array at a specific x and y pixel location. 
void PNGWriter::drawPixelImage(int x, int y, int red, int green, int blue, int alpha){
            int offset = (y * width + x) * 4;
            imageData[offset + 0] = red; // red
            imageData[offset + 1] = green; // green
            imageData[offset + 2] = blue; // blue
            imageData[offset + 3] = alpha; // alpha (keep at 255 for no transparency)
  
}

/*
Writes the imageData to a png file with the given name.
Param: filename including ".png"
*/
void PNGWriter::writePNGFile(char *filename) {
    FILE *fp = fopen(filename, "wb");
    if(!fp) abort();
    
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();
    
    png_infop info = png_create_info_struct(png);
    if (!info) abort();
    
    if (setjmp(png_jmpbuf(png))) abort();
    
    png_init_io(png, fp);
    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png,
        info,
        width, height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    
    // pnglib requires the image stored as rows
    for(int y = 0; y < height; y++) {
        imageRows[y] = &imageData[y * width * 4];
    }
    
    // These three commands write the png file
    png_write_info(png, info);
    png_write_image(png, imageRows);
    png_write_end(png, NULL);
    
    fclose(fp);

    png_destroy_write_struct(&png, &info);
}