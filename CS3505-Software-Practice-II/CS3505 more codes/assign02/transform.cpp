#include "transform.h"
#include "point.h"
#include <iostream> // for ostream

/**
 * Transform class: This class is used to transform one 2-D point into a different 2-D point.
 * by Jacob Xu
*/

//A constructor that has six double-type parameters in this order:a,b,c,d,e,f
Transform::Transform(double a, double b, double c, double d, double e, double f): a(a), b(b), c(c), d(d), e(e), f(f) {}
//Make a method named getParameters that takes one parameter that is an array of doubles.
void Transform::getParameters(double parameter[6]){
    parameter[0] = a;
    parameter[1] = b;
    parameter[2] = c;
    parameter[3] = d;
    parameter[4] = e;
    parameter[5] = f;
}
//Overload the * operator that has a Point parameter and returns a transformed point
Point Transform::operator*(Point& pt) {
    double transfX = a * pt.getX() + b * pt.getY() + c;
    double transfY = d * pt.getX() + e * pt.getY() + f;
    return Point(transfX, transfY);
}
//Overload the *= operator Note that this requires operator*= to be a friend function of both Point and Transform classes
Point& operator*=(Point& pt, Transform& tf){
    double transfX = tf.a * pt.getX() + tf.b * pt.getY() + tf.c;
    double transfY = tf.d * pt.getX() + tf.e * pt.getY() + tf.f;
    pt = Point(transfX, transfY);
    return pt;
}
//Overload the insertion operator << for a Transform object. Refer to the version for the Point class for an example. The output string should be like "[a, b, c, d, e, f]"
std::ostream& operator<<(std::ostream& output, Transform& tf){
    output << "[" << tf.a << "," << tf.b << "," << tf.c << "," << tf.d << "," << tf.e << "," << tf.f << "]";
    return output; 
}
