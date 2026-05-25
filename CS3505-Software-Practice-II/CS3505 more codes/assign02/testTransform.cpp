#include <iostream>
#include "point.h"
#include "transform.h"
/**
 * Make a test program testTransform.cpp. 
 * by Jacob Xu
*/
int main() {
    // Construct a Point and a Transform object
    Point thePoint(1.0, 0.0);
    Transform theTransform1(1.0113, -0.1788, 0.0, 0.1788, 1.0113, 0.0);
    //Then choose a different set of transform parameters and compile/run it again. Note that values much bigger than 1.0 
    //will cause things to grow very large very fast, so stick with numbers less than about 1.1
    Transform theTransform2(1.0624, -0.0664, 0.0, 0.3412, 1.0044, 0.0);
    // Loop 200 times, applying the transform to the point using the *= operator,
    for (int i = 0; i < 200; ++i) {
        //thePoint *= theTransform1;
        thePoint *= theTransform2;
        //then printing the point using its overloaded << operator as well as a newline (std::endl) at each iteration.
        std::cout << thePoint << std::endl;
    }

    return 0;
}