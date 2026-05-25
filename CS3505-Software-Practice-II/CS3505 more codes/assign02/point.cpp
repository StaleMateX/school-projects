#include "point.h"
#include <iostream>
/*
Point class for CS 3505 lecture 3.
A Point holds x and y values.
By Eric Heisler and Jacob Xu
*/

// Implement the methods prototyped in point.h
// You can include comments here if desired, 
// but they are already documented in point.h

Point::Point() {}

Point::Point(double initx, double inity) : x(initx), y(inity) {}

Point::~Point() {}

double Point::getX() {
    return x;
}

double Point::getY() {
    return y;
}

// Operators
Point Point::operator*(double scaleFactor) {
	Point res(x*scaleFactor, y*scaleFactor);
	return res;
}

// This doesn't start with Point:: so it is not class code.
std::ostream& operator<<(std::ostream& output, Point pt) {
    output << pt.x << ", " << pt.y;
    return output;
}
