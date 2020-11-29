/**
Computer Graphics, Tsinghua University, cube.cpp
Purpose: Define functions and private variables for cube objects

@author Evan Heetderks
@version 1.1 05/14/17
*/

#include "cube.h"

/**
Default cube contstructor: creates and initializes a cube to null values
@author Evan Heetderks
*/
Cube::Cube() {
	frontColor = NULL;
	backColor = NULL;
	leftColor = NULL;
	rightColor = NULL;
	topColor = NULL;
	bottomColor = NULL;

	frontFace = NULL;
	backFace = NULL;
	topFace = NULL;
	bottomFace = NULL;
	rightFace = NULL;
	leftFace = NULL;

	xLayer = NULL;
	yLayer = NULL;
	zLayer = NULL;


	xOrigin = NULL;
	yOrigin = NULL;
	zOrigin = NULL;

	xRot = NULL;
	yRot = NULL;
	zRot = NULL;
}

/**
Explicit cube contstructor: creates and initializes a cube based on its ID
@author Evan Heetderks
@param x: the x portion of the ID
@param y: the y portion of the ID
@param z: the z portion of the ID
@param colorTextures: an array of the 7 possible color textures
*/
Cube::Cube(int x, int y, int z, GLuint colorTextures[7]) {
	// Front face list
	frontFace = glGenLists(6);
	glNewList(frontFace, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();
	glEndList();

	// Back face list
	backFace = frontFace + 1;
	glNewList(backFace, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();
	glEndList();

	// Top face list
	topFace = backFace + 1;
	glNewList(topFace, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();
	glEndList();

	// Bottom face list
	bottomFace = topFace + 1;
	glNewList(bottomFace, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
	glEndList();

	// Right face list
	rightFace = bottomFace + 1;
	glNewList(rightFace, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();
	glEndList();

	// Left face list
	leftFace = rightFace + 1;
	glNewList(leftFace, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	glEndList();
	
	// Back face color
	if (z == 0) {
		backColor = colorTextures[1];
	}
	else {
		backColor = colorTextures[6];
	}

	// Front face color
	if (z == 2) {
		frontColor = colorTextures[0];
	}
	else {
		frontColor = colorTextures[6];
	}

	// Top face color
	if (y == 2) {
		topColor = colorTextures[2];
	}
	else {
		topColor = colorTextures[6];
	}

	// Bottom face color
	if (y == 0) {
		bottomColor = colorTextures[3];
	}
	else {
		bottomColor = colorTextures[6];
	}

	// Right face color
	if (x == 2) {
		rightColor = colorTextures[4];
	}
	else {
		rightColor = colorTextures[6];
	}

	// Left face color
	if (x == 0) {
		leftColor = colorTextures[5];
	}
	else {
		leftColor = colorTextures[6];
	}

	// Default origin position
	xOrigin = ((2*x) - 2)*1.0f;
	yOrigin = ((2*y) - 2)*1.0f;
	zOrigin = ((2*z) - 2)*1.0f;

	// Cube rotation neutral in its original position
	xRot = 0;
	yRot = 0;
	zRot = 0;

	xLayer = x;
	yLayer = y;
	zLayer = z;
}

/**
Renders a cube with appropriate translation, rotation, and textures
@author Evan Heetderks
*/
void Cube::render() {
	// Start with a fresh identity matrix
	glPushMatrix();

	// Rotate around the x, y, or z axes
	glRotatef((float)xRot, 1.0f, 0.0f, 0.0f);
	glRotatef((float)yRot, 0.0f, 1.0f, 0.0f);
	glRotatef((float)zRot, 0.0f, 0.0f, 1.0f);

	// Render cube at designated point of origin
	glTranslatef(xOrigin, yOrigin, zOrigin);

	// Render each cube face with the appropriate color
	glBindTexture(GL_TEXTURE_2D, frontColor);
	glCallList(frontFace);
	glBindTexture(GL_TEXTURE_2D, backColor);
	glCallList(backFace);
	glBindTexture(GL_TEXTURE_2D, topColor);
	glCallList(topFace);
	glBindTexture(GL_TEXTURE_2D, bottomColor);
	glCallList(bottomFace);
	glBindTexture(GL_TEXTURE_2D, rightColor);
	glCallList(rightFace);
	glBindTexture(GL_TEXTURE_2D, leftColor);
	glCallList(leftFace);

	// Finish rendering
	glPopMatrix();
}

/**
Updates a cubes rotation around the x axis
@author Evan Heetderks
@param angle: the increase in degrees
*/
void Cube::setXRot(int angle) {
	xRot = xRot + angle;
}

/**
Updates a cubes rotation around the y axis
@author Evan Heetderks
@param angle: the increase in degrees
*/
void Cube::setYRot(int angle) {
	yRot = yRot + angle;
}

/**
Updates a cubes rotation around the z axis
@author Evan Heetderks
@param angle: the increase in degrees
*/
void Cube::setZRot(int angle) {
	zRot = zRot + angle;
}

/**
Updates a cube's origin, layer numbers, and textures based on rotation around the x axis
@author Evan Heetderks
@param quarterRotations: the number of quarter rotations around the x axis (can be positive or negative)
*/
void Cube::xRotUpdate(int quarterRotations) {
	// Update the matrix based on x rotation
	xRotMatrix[0] = cos(-xRot*PI / 180);
	xRotMatrix[1] = sin(-xRot*PI / 180);
	xRotMatrix[2] = -sin(-xRot*PI / 180);
	xRotMatrix[3] = cos(-xRot*PI / 180);

	// Calculate the new y and z positions
	int newYPos = (xRotMatrix[0] * yOrigin) + (xRotMatrix[1] * zOrigin);
	int newZPos = (xRotMatrix[2] * yOrigin) + (xRotMatrix[3] * zOrigin);

	// Calculate which x-layer the cube is in
	if (newYPos <= -1.00000) {
		yLayer = 0;
	}
	else if (newYPos >= 1.00000) {
		yLayer = 2;
	}
	else {
		yLayer = 1;
	}

	// Calculate which y-layer the cube is in
	if (newZPos <= -1.00000) {
		zLayer = 0;
	}
	else if (newZPos >= 1.00000) {
		zLayer = 2;
	}
	else {
		zLayer = 1;
	}

	// Reset the origin location and x rotation, shift the textures around the x axis
	yOrigin = newYPos;
	zOrigin = newZPos;
	xRot = 0;
	shiftTextures(quarterRotations, 0, 0);
}

/**
Updates a cube's origin, layer numbers, and textures based on rotation around the y axis
@author Evan Heetderks
@param quarterRotations: the number of quarter rotations around the y axis (can be positive or negative)
*/
void Cube::yRotUpdate(int quarterRotations) {
	// Update the matrix based on y rotation
	yRotMatrix[0] = cos(-yRot*PI / 180);
	yRotMatrix[1] = -sin(-yRot*PI / 180);
	yRotMatrix[2] = sin(-yRot*PI / 180);
	yRotMatrix[3] = cos(-yRot*PI / 180);

	// Calculate the new x and z positions
	int newXPos = (yRotMatrix[0] * xOrigin) + (yRotMatrix[1] * zOrigin);
	int newZPos = (yRotMatrix[2] * xOrigin) + (yRotMatrix[3] * zOrigin);

	// Calculate which x-layer the cube is in
	if (newXPos <= -1.00000) {
		xLayer = 0;
	}
	else if (newXPos >= 1.00000) {
		xLayer = 2;
	}
	else {
		xLayer = 1;
	}

	// Calculate which y-layer the cube is in
	if (newZPos <= -1.00000) {
		zLayer = 0;
	}
	else if (newZPos >= 1.00000) {
		zLayer = 2;
	}
	else {
		zLayer = 1;
	}

	// Reset the origin location and y rotation, shift the textures around the y axis
	xOrigin = newXPos;
	zOrigin = newZPos;
	yRot = 0;
	shiftTextures(0, quarterRotations, 0);
}

/**
Updates a cube's origin, layer numbers, and textures based on rotation around the z axis
@author Evan Heetderks
@param quarterRotations: the number of quarter rotations around the z axis (can be positive or negative)
*/
void Cube::zRotUpdate(int quarterRotations) {
	// Update the z rotation matrix based on z rotation
	zRotMatrix[0] = cos(-zRot*PI / 180);
	zRotMatrix[1] = sin(-zRot*PI / 180);
	zRotMatrix[2] = -sin(-zRot*PI / 180);
	zRotMatrix[3] = cos(-zRot*PI / 180);

	// Calculate the new x and y positions
	int newXPos = (zRotMatrix[0] * xOrigin) + (zRotMatrix[1] * yOrigin);
	int newYPos = (zRotMatrix[2] * xOrigin) + (zRotMatrix[3] * yOrigin);

	// Calculate which x-layer the cube is in
	if (newXPos <= -1.00000) {
		xLayer = 0;
	}
	else if (newXPos >= 1.00000) {
		xLayer = 2;
	}
	else {
		xLayer = 1;
	}

	// Calculate which y-layer the cube is in
	if (newYPos <= -1.00000) {
		yLayer = 0;
	}
	else if (newYPos >= 1.00000) {
		yLayer = 2;
	}
	else {
		yLayer = 1;
	}

	// Reset the origin location and z rotation, shift the textures around the z axis
	xOrigin = newXPos;
	yOrigin = newYPos;
	zRot = 0;
	shiftTextures(0, 0, quarterRotations);
}

/**
Slides a cubes textures around the x, y, or z axis
@author Evan Heetderks
@param x: number of slide interations around the x axis
@param y: number of slide interations around the y axis
@param z: number of slide interations around the z axis
*/
void Cube::shiftTextures(int x, int y, int z) {
	if (x > 0) {
		for (int i = 0; i < x; i++) {
			GLuint temp = frontColor;
			frontColor = topColor;
			topColor = backColor;
			backColor = bottomColor;
			bottomColor = temp;
		}
	}
	if (x < 0) {
		for (int i = 0; i < x + 4; i++) {
			GLuint temp = frontColor;
			frontColor = topColor;
			topColor = backColor;
			backColor = bottomColor;
			bottomColor = temp;
		}
	}

	if (y > 0) {
		for (int i = 0; i < y; i++) {
			GLuint temp = frontColor;
			frontColor = leftColor;
			leftColor = backColor;
			backColor = rightColor;
			rightColor = temp;
		}
	}

	if (y < 0) {
		for (int i = 0; i < y + 4; i++) {
			GLuint temp = frontColor;
			frontColor = leftColor;
			leftColor = backColor;
			backColor = rightColor;
			rightColor = temp;
		}
	}

	if (z > 0) {
		for (int i = 0; i < z; i++) {
			GLuint temp = topColor;
			topColor = rightColor;
			rightColor = bottomColor;
			bottomColor = leftColor;
			leftColor = temp;
		}
	}

	if (z < 0) {
		for (int i = 0; i < z + 4; i++) {
			GLuint temp = topColor;
			topColor = rightColor;
			rightColor = bottomColor;
			bottomColor = leftColor;
			leftColor = temp;
		}
	}
}

/**
Returns the number of the x layer a cube lies in
@author Evan Heetderks
@return xLayer: the number of the x layer a cube lies in
*/
int Cube::getXLayer() {
	return xLayer;
}

/**
Returns the number of the y layer a cube lies in
@author Evan Heetderks
@return yLayer: the number of the y layer a cube lies in
*/
int Cube::getYLayer() {
	return yLayer;
}

/**
Returns the number of the z layer a cube lies in
@author Evan Heetderks
@return zLayer: the number of the z layer a cube lies in
*/
int Cube::getZLayer() {
	return zLayer;
}

/**
Returns the x rotation angle of a cube
@author Evan Heetderks
@return xRrot: the x rotation angle of a cube
*/
int Cube::getXRot() {
	return xRot;
}

/**
Returns the y rotation angle of a cube
@author Evan Heetderks
@return yRrot: the y rotation angle of a cube
*/
int Cube::getYRot() {
	return yRot;
}

/**
Returns the z rotation angle of a cube
@author Evan Heetderks
@return zRrot: the z rotation angle of a cube
*/
int Cube::getZRot() {
	return zRot;
}