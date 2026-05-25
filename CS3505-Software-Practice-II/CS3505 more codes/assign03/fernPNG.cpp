#include "point.h"
#include "transform.h"
#include "pngWriter.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

/**
 * Write a test program fernPNG.cpp to make executable fernPNG. 
 * This will be structured similar to your fern.cpp file from assignment 1, 
 * so you can borrow a lot of ideas from there. It will set up four Transformation objects and one Point object, 
 * perform a number of iterations in which a Transform is randomly selected with some probability and 
 * then applied to the Point, 
 * and set a pixel in the png image at each iteration.
 * 
 * by Jacob Xu
*/

const double xMax = 2.75; // keep xMax and yMax at these values
const double yMax = 10.5;

int main(int argc, char *argv[]){
    //if the input is not in this format, which is 4 string then it will throw a message and out of main
    if(argc != 5){
        std::cout << "Requires 4 arguments: filename, width, height, number of iterations" << std::endl;
        return 0;
    }

    //std::stringstream outputFilename(argv[1]);

    //setting up width, height, iteration and then convert them to int
    int widths, heights, iterations;
    std::stringstream widthStream(argv[2]);
    std::stringstream heightStream(argv[3]);
    std::stringstream interationStream(argv[4]);
    widthStream >> widths;
    heightStream >> heights;
    interationStream >> iterations;

    //if width, height, iteration is less than 1 then throw message and out of main
    if(widths < 1 || heights < 1 || iterations < 1){
        std::cout << "Width, height, number of iterations were not valid values > 0" << std::endl;
        return 0;
    }
    //call PNGWriter from pngWriter
    PNGWriter pngWriter(widths, heights);

    //Initialize Transform and Point objects
    Transform transform1(0.00, 0.00, 0.00, 0.00, 0.16, 0.00);  //with %1 probablity
    Transform transform2(0.85, 0.04, 0.00, -0.04, 0.85, 1.6);  //with %85 probablity
    Transform transform3(0.20, -0.26, 0.00, 0.23, 0.22, 1.60); //with %7 probablity
    Transform transform4(-0.15, 0.28, 0.00, 0.26, 0.24, 0.44); //with %7 probablity

    //Start with an initial point
    Point point(0, 0);

    unsigned char image[widths * heights / 8]{};

    //calculate the correct point for x and y then set them to pixel
    for(int i = 0; i < iterations; i++){
        int random = rand() % 100; //generate a random number from 0 to 99
        Transform selectedTransform;
        if(random < 1){ //follow the idea from wikipedia (https://en.wikipedia.org/wiki/Barnsley_fern)
            selectedTransform = transform1;
        }else if(random < 86){
            selectedTransform = transform2;
        }else if(random < 93){
            selectedTransform = transform3;
        }else{
            selectedTransform = transform4;
        }
        // Apply the selected transform to the point
        point *= selectedTransform;

        int pixelX = widths / 2 + (int)(widths * point.getX() / xMax / 2);//from canvas guide
        int pixelY = (int)(heights * point.getY() / yMax);

        //get rid of top two line because didn't pass on autograder, reverse it from left to right in draw method instead
        int byteN = pixelY * (widths / 8) + pixelX / 8; 
        int bitN = pixelX % 8;  
        //bitwise OR operation that sets the bit at position bitN to 1 in the byteN
        image[byteN] |= (1 << bitN); //idea from x = x | (1 << 5) sets the sixth bit in x, resulting in x = 00101001
    }

    // Draw the point in the PNG image
    for (int col = 0; col < widths; col++) {
        for (int row = 0; row < heights; row++) {  // Iterate from top to bottom
            //from fern assignment get the pixel
            int byteN = row * (widths / 8) + col / 8;
            int bitN = col % 8;
            bool boo = image[byteN] & (1 << bitN);
            if(boo != 0){
                pngWriter.drawPixelImage(col, heights - 1 - row, 0, 255, 0, 255); //green
            }else{
                pngWriter.drawPixelImage(col, heights - 1 - row, 0, 0, 0, 0);   //black 
            }
        }
    }
        
    // Save the image to a PNG file
    pngWriter.writePNGFile(argv[1]);

    return 0;
}
