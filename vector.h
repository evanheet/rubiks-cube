/**
Computer Graphics, Tsinghua University, vector.h
Purpose: Declare functions and private variables for vector objects

@author Evan Heetderks
@version 1.1 05/14/17
*/

#ifndef Vector_HEADER
#define Vector_HEADER

#include <math.h>
# define PI  3.14159265358979323846

class Vector {
public:
	Vector();
	Vector(double x, double y, double z);
	void set(double x, double y, double z);
	double dotProduct(Vector vector);
	double angleBetween(Vector vector);
	Vector subtract(Vector vector);
	Vector scale(Vector startingPoint, double scaleFactor);
	double getXValue();
	double getYValue();
	double getZValue();
private:
	double xValue;
	double yValue;
	double zValue;
};

#endif
