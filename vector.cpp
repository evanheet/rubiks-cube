/**
Computer Graphics, Tsinghua University, vector.cpp
Purpose: Define functions and private variables for vector objects

@author Evan Heetderks
@version 1.1 05/14/17
*/

#include "vector.h"

Vector::Vector() {

}

/**
Explicit vector contstructor: creates a vector and assigns values to its xyz compoments
@author Evan Heetderks
@param x: the x component of the vector
@param y: the y component of the vector
@param z: the z component of the vector
*/
Vector::Vector(double x, double y, double z) {
	xValue = x;
	yValue = y;
	zValue = z;
}

/**
Changes the xyz components of an existing vector
@author Evan Heetderks
@param x: the new x component of the vector
@param y: the new y component of the vector
@param z: the new z component of the vector
*/
void Vector::set(double x, double y, double z) {
	xValue = x;
	yValue = y;
	zValue = z;
}

/**
Returns the dot product of two existing vectors
@author Evan Heetderks
@param vector: the vector with which the selected vector will be taken the dot product with
@return the dot product of the two cectors
*/
double Vector::dotProduct(Vector vector) {
	return (xValue * vector.getXValue()) + (yValue * vector.getYValue()) + (zValue * vector.getZValue());
}

/**
Returns the angle between two existing vectors
@author Evan Heetderks
@param vector: the vector with which the angle between the selected vector will be taken
@return the dot product of the two cectors
*/
double Vector::angleBetween(Vector vector) {
	return (180/PI)*acos(dotProduct(vector)/(sqrt(pow(xValue, 2) + pow(yValue, 2) + pow(zValue, 2))   * 
				sqrt(pow(vector.getXValue(), 2) + pow(vector.getYValue(), 2) + pow(vector.getZValue(), 2))));
}

/**
Returns a resultant vector of two vectors being subtracted
@author Evan Heetderks
@param vector: the vector that will the subtracted from the existing vector
@return returnVector: the resultant vector of the subtraction
*/
Vector Vector::subtract(Vector vector) {
	Vector returnVector(xValue - vector.getXValue(), yValue - vector.getYValue(), zValue - vector.getZValue());
	return returnVector;
}

/**
Returns a resultant vector corresponding to a scaled vector
@author Evan Heetderks
@param startingPoint: the starting point of the existing vector
@param scaleFactor: the factor by which the xyz components of the existing vector will be multiplied
@return returnVector: the resultant vector of the scaling
*/
Vector Vector::scale(Vector startingPoint, double scaleFactor) {
	Vector returnVector((scaleFactor * xValue) + startingPoint.getXValue(),
						(scaleFactor * yValue) + startingPoint.getYValue(),
						(scaleFactor * zValue) + startingPoint.getZValue());
	return returnVector;
}

/**
Returns the x component of an existing vector
@author Evan Heetderks
@return xValue: the x component of a vector
*/
double Vector::getXValue() {
	return xValue;
}

/**
Returns the y component of an existing vector
@author Evan Heetderks
@return xValue: the y component of a vector
*/
double Vector::getYValue() {
	return yValue;
}

/**
Returns the z component of an existing vector
@author Evan Heetderks
@return xValue: the z component of a vector
*/
double Vector::getZValue() {
	return zValue;
}