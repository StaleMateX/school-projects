/**
 * Jacob Xu
 * Assignment 1: Barnsley fern
 * The program will:
 * Ask the user for a number of iterations to run the IFS. It will then read a number input by the user. If the number is not positive, a helpful usage message will be printed, and the program will end (returning from the main function with a value of zero).
 * Beginning with an initial point of (0.0, 0.0), it will perform the iterations described above, selecting one of the four functions randomly with the corresponding probability and computing the next value of x and y.
 * With each iteration, the (x, y) coordinates will be added to an array of "pixels". More on this below.
 * After completing all iterations, the array of "pixels" will be drawn in the terminal by printing a character for each pixel that is set, and empty space for pixels that are not set.
*/
#include <iostream>
#include <cstdlib>
using namespace std;

const int width = 120; // set width and height as desired
const int height = 120;
const double xMax = 2.75; // keep xMax and yMax at these values
const double yMax = 10.5;

//A very good strategy here is to store all of your coefficients in a global const array of arrays.
const double transformations[][6] = {
        {0.00, 0.00, 0.00, 0.00, 0.16, 0.00}, //with %1 probablity
        {0.85, 0.04, 0.00, -0.04, 0.85, 1.6}, //with %85 probablity
        {0.20, -0.26, 0.00, 0.23, 0.22, 1.60}, //with %7 probablity
        {-0.15, 0.28, 0.00, 0.26, 0.24, 0.44}  //with %7 probablity
    };

//Randomly selects one of the four transformations with the corresponding probabilities, returning 0, 1, 2, or 3. 
int selectTransformation(){
    int random = rand() % 100; //generate a random number from 0 to 99
    if(random < 1){ //follow the idea from wikipedia (https://en.wikipedia.org/wiki/Barnsley_fern)
        return 0;
    }else if(random < 86){
        return 1;
    }else if(random < 93){
        return 2;
    }else{
        return 3;
    }
}

//The given transformation is applied to the values in x and y. The resulting values are stored in the same x and y variables.
void applyTransformation(double& x, double& y, const double* transformation){
    double tempX = x;//create a temp x and y
    double tempY = y;
    // fixed for test 5 on Autograder
    x = transformation[0] * tempX + transformation[1] * tempY + transformation[2];//follow by the canvas guide
    y = transformation[3] * tempX + transformation[4] * tempY + transformation[5];
}

//Sets the pixel that contains the point (x, y) to be drawn in the image.
void setPixel(double x, double y, unsigned char* image){
    int pixelX = width / 2 + (int)(width * x / xMax / 2);//from canvas guide
    int pixelY = (int)(height * y / yMax);

    // int byteN = pixelY * (width / 8) + (width - 1 - pixelX) / 8; //An image with a given width and height will require width * height / 8 bytes as long as either width or height is a multiple of 8
    // int bitN = (width - 1 - pixelX) % 8; //the picture was reverse from left to right, use (width - 1 - pixelX) to change it to the same direction from canvas
    //get rid of top two line because didn't pass on autograder, reverse it from left to right in draw method instead
    int byteN = pixelY * (width / 8) + pixelX / 8; 
    int bitN = pixelX % 8;  
    //bitwise OR operation that sets the bit at position bitN to 1 in the byteN
    image[byteN] |= (1 << bitN); //idea from x = x | (1 << 5) sets the sixth bit in x, resulting in x = 00101001
}

//Returns true if the pixel at the given row and column in the image is set. In other words, that bit has value 1.
bool getPixel(int row, int col, unsigned char* image){
    int byteN = row * (width / 8) + col / 8;
    int bitN = col % 8;
    //checks if the bit at position bitN in the byteN byte is set
    bool boo = (image[byteN] & (1 << bitN)) !=0; //y = x & (1 << n) results in y = 0 if bit (n-1) in x is 0, and y > 0 if that bit is 1
    return boo;
}

//Draw the image one row at a time by printing to the terminal.
void drawImage(unsigned char* image){
    for(int row = height - 1; row >= 0; row--){//Draw the image one row at a time by printing to the terminal.
        for (int col = 0; col < width; col++){  // Iterate from left to right (new update)
            if(getPixel(row,col,image)){
                cout << "#";
            }else{
                cout << " ";
            }
            //If a pixel is set, print "#". If the pixel is not set, print " " (one empty space).
        }
        cout << endl;//create a new line
    }
}

//Performs the specified number of iterations by repeatedly selecting a transformation function, applying that transformation, 
//and setting the pixel based on the resulting x and y values.
void performIterations(int iterations, unsigned char* image){
    //Beginning with an initial point of (0.0, 0.0)
    double x = 0.0;
    double y = 0.0;

    for(int i = 0; i < iterations; ++i){//loop from iterations
        // fixed for test 5 on Autograder
        int specifiedNum = selectTransformation();//get the number of selectTransformation
        applyTransformation(x, y, transformations[specifiedNum]);//apply it 
        setPixel(x,y,image);//set it
    }
}

//It will handle getting the number of iterations from the user and delegating the rest of the work to the other functions. 
//You will also create the pixel array described above at the beginning of main.
int main(){

    //initialize the number of iterations
    int iterations;

    cout << "Enter the number of iterations to run the IFS: ";
    cin >> iterations;//get the iterations

    //If the number is not positive, a helpful usage message will be printed
    if(iterations <= 0){
        cout << "Please enter positive iterations. End program." << endl;
        return 0;//the program will end (returning from the main function with a value of zero).
    }
    
    //An image with a given width and height will require width * height / 8 bytes as long as either width or height is a multiple of 8
    unsigned char image[width*height/8]{};

    performIterations(iterations, image); // call performIteration method then draw it
    drawImage(image);
    return 0;
}