/**
Computer Graphics, Tsinghua University, cube.h
Purpose: Declare functions and private variables for cube objects

@author Evan Heetderks
@version 1.1 05/14/17
*/

#ifndef Cube_HEADER
#define Cube_HEADER

#include <math.h>
#include <gl/glut.h>
#include <gL/glu.h>
#include <gL/gl.h>
# define PI  3.14159265358979323846

class Cube {
public:
	Cube();
	Cube(int x, int y, int z, GLuint colorTextures[7]);
	void render();
	void setXRot(int angle);
	void setYRot(int angle);
	void setZRot(int angle);
	void xRotUpdate(int quarterRotations);
	void yRotUpdate(int quarterRotations);
	void zRotUpdate(int quarterRotations);
	void shiftTextures(int x, int y, int z);;
	int getXLayer();
	int getYLayer();
	int getZLayer();
	int getXRot();
	int getYRot();
	int getZRot();
private:
	GLuint frontColor;
	GLuint backColor;
	GLuint leftColor;
	GLuint rightColor;
	GLuint topColor;
	GLuint bottomColor;

	GLuint frontFace;
	GLuint backFace;
	GLuint topFace;
	GLuint bottomFace;
	GLuint rightFace;
	GLuint leftFace;

	int xLayer;
	int yLayer;
	int zLayer;

	float xRotMatrix[4];
	float yRotMatrix[4];
	float zRotMatrix[4];

	int xRot;
	int yRot;
	int zRot;

	float xOrigin;
	float yOrigin;
	float zOrigin;
};

#endif