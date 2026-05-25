#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <iostream> // for ostream
#include "point.h"

/**
 * Transform header class
 * by Jacob Xu
*/

class Transform{
    //Instance variables to hold six double-type values. You can use six separate variables or an array as desired.
    private: double a, b, c, d, e, f;
    public:
        // Default constructor
        Transform() : a(0), b(0), c(0), d(0), e(0), f(0) {}
        //A constructor that has six double-type parameters in this order:
        Transform(double a, double b, double c, double d, double e, double f);
        // The method will set the six parameters in the array in the order [a, b, c, d, e, f]. This method has void return type.
        void getParameters(double parameter[6]);
        //Overload the * operator that has a Point parameter and returns a transformed point.
        Point operator*(Point& pt);
        //Overload the *= operator corresponding to Point& operator*=(Point& pt, Transform tr)
        friend Point& operator*=(Point& pt, Transform& tf);
        //Overload the insertion operator << for a Transform object.
        friend std::ostream& operator<<(std::ostream& output, Transform& tf);

};

#endif