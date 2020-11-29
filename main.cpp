/**
Computer Graphics, Tsinghua University, main.cpp
Purpose: Organize cube objects, their functions, and mouse behavior to
		 create a fully functional rubix cube

@author Evan Heetderks
@version 1.1 05/14/17
*/

#include <gl/glut.h>
#include <gL/glu.h>
#include <gL/gl.h>
#include "SOIL.h"
#include "cube.h"
#include "vector.h"
#include <time.h> 

// Create a 3-D array of 27 sub-cubes
Cube cubes[3][3][3];

/*	Array for all 7 color textures
	[0] - red
	[1] - orange
	[2] - yellow
	[3] - green
	[4] - blue
	[5] - white
	[6] - black
*/
GLuint colorTextures[7];

// Distance from camera to center of rubix cube at any given time
int cameraRadius;

float phi;			// Camera position angle w/ respect to x-z plane at center of the rubix cube
float theta;		// Camera position angle w/ respect to y-z plane at center of the rubix cube

// Vectors representing the camera position and the direction it is facing
Vector cameraPosition;
Vector cameraDirection;

int mouseState;		// Whether a mouse button is being pressed
int mouseButton;	// Mouse button being pressed
int mouseX;			// x-coordinate of mouse when pressed
int mouseY;			// y-coordinate of mouse when pressed

// Arrays to store incoming GL matrix data
GLdouble modelViewMatrix[16];
GLdouble projectionMatrix[16];
GLint viewport[4];

// Position of the mouse mapped onto the rubix cube
GLdouble objX;
GLdouble objY;
GLdouble objZ;

// Array containing the x, y, and z ID numbers of the 9 cubes in any given layer
int selectedCubeIDs[9][3];

// Whether a layer is being rotated around the x, y, or z axes
bool xRotation;
bool yRotation;
bool zRotation;

// Whether a layer is being rotated
bool layerBeingSet;

// Corrected rotation value
int rotationDirection;

// Normal vector to each of the 6 cube faces
Vector topNormal;
Vector bottomNormal;
Vector leftNormal;
Vector rightNormal;
Vector frontNormal;
Vector backNormal;

GLdouble rotationPointX;
GLdouble rotationPointY;
GLdouble rotationPointZ;

// (x,y,z) location of the center point of each of the 6 faces
Vector topCenterPoint;
Vector bottomCenterPoint;
Vector leftCenterPoint;
Vector rightCenterPoint;
Vector frontCenterPoint;
Vector backCenterPoint;


// View reference vectors pointing from the rubix cube center to the point bisecting each edge of the rubix cube
// (needed to determine which face the camera is facing)
Vector posYposZ;
Vector negYposZ;
Vector negXposZ;
Vector posXposZ;
Vector posXposY;
Vector posXnegY;
Vector negXnegY;
Vector negXposY;
Vector posYnegZ;
Vector negYnegZ;
Vector negXnegZ;
Vector posXnegZ;

/**
Main initializer: sets the original scene for the program
@author Evan Heetderks
*/
void initialize() {
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Generate sub-cube textures
	glGenTextures(1, &colorTextures[0]);
	glGenTextures(1, &colorTextures[1]);
	glGenTextures(1, &colorTextures[2]);
	glGenTextures(1, &colorTextures[3]);
	glGenTextures(1, &colorTextures[4]);
	glGenTextures(1, &colorTextures[5]);
	glGenTextures(1, &colorTextures[6]);

	// Load the textures using the SOIL library
	colorTextures[0] = SOIL_load_OGL_texture("red.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	colorTextures[1] = SOIL_load_OGL_texture("orange.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	colorTextures[2] = SOIL_load_OGL_texture("yellow.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	colorTextures[3] = SOIL_load_OGL_texture("green.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	colorTextures[4] = SOIL_load_OGL_texture("blue.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	colorTextures[5] = SOIL_load_OGL_texture("white.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	colorTextures[6] = SOIL_load_OGL_texture("black.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);

	// Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	// Create each of the 27 cubes in the rubix cube
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				cubes[i][j][k] = Cube(i, j, k, colorTextures);
			}
		}
	}

	// Create the normal vectors to each face
	topNormal = Vector(0, 1, 0);
	bottomNormal = Vector(0, -1, 0);
	leftNormal = Vector(-1, 0, 0);
	rightNormal = Vector(1, 0, 0);
	frontNormal = Vector(0, 0, 1);
	backNormal = Vector(0, 0, -1);

	topCenterPoint = Vector(0, 3, 0);
	bottomCenterPoint = Vector(0, -3, 0);
	leftCenterPoint = Vector(-3, 0, 0);
	rightCenterPoint = Vector(3, 0, 0);
	frontCenterPoint = Vector(0, 0, 3);
	backCenterPoint = Vector(0, 0, -3);

	// Create the view reference vectors
	posYposZ = Vector(0, 1, 1);
	negYposZ = Vector(0, -1, 1);
	negXposZ = Vector(-1, 0, 1);
	posXposZ = Vector(1, 0, 1);
	posXposY = Vector(1, 1, 0);
	posXnegY = Vector(1, -1, 0);
	negXnegY = Vector(-1, -1, 0);
	negXposY = Vector(-1, 1, 0);
	posYnegZ = Vector(0, 1, -1);
	negYnegZ = Vector(0, -1, -1);
	negXnegZ = Vector(-1, 0, -1);
	posXnegZ = Vector(1, 0, -1);

	// Set default camera radius and angles
	cameraRadius = 13;
	theta = (float)PI / 4;
	phi = (float)PI / 5;

	// Calculate default camera position
	cameraPosition.set(cameraRadius * sin(theta) * cos(phi), cameraRadius * sin(phi), cameraRadius * cos(theta) * cos(phi));

	// All cubes are static at first
	layerBeingSet = false;
}

/**
Main display function to render each scene
@author Evan Heetderks
*/
void displayFunc() {
	// Reset buffers and coordinate system
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Set camera position and orientation
	if (phi >= (3*PI/2) || phi < (PI/2)) {
		// Camera rightside up if 3pi/2 <= phi < pi/2
		gluLookAt(cameraPosition.getXValue(), cameraPosition.getYValue(), cameraPosition.getZValue(), 0, 0, 0, 0.0f, 1.0f, 0.0f);
	}
	else {
		// Camera upside down if pi/2 <= phi < 3pi/2
		gluLookAt(cameraPosition.getXValue(), cameraPosition.getYValue(), cameraPosition.getZValue(), 0, 0, 0, 0.0f, -1.0f, 0.0f);
	}
	
	// Render all the sub-cubes
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				cubes[x][y][z].render();
			}
		}
	}

	glutSwapBuffers();
}

/**
Rotates cubes identified by a layer selection
@param angle: the desired change in rotation
@param rotX: whether rotation is done about x axis
@param rotY: whether rotation is done about y axis
@param rotZ: whether rotation is done about z axis
@param id: array containing the x, y, and z ID numbers of the cubes in a selected layer
@author Evan Heetderks
*/
void rotateCubes(int angle, bool rotX, bool rotY, bool rotZ, int id[9][3]) {
	if (rotX) {
		for (int i = 0; i < 9; i++) {
			cubes[id[i][0]][id[i][1]][id[i][2]].setXRot(angle);
		}
	}
	if (rotY) {
		for (int i = 0; i < 9; i++) {
			cubes[id[i][0]][id[i][1]][id[i][2]].setYRot(angle);
		}
	}
	if (rotZ) {
		for (int i = 0; i < 9; i++) {
			cubes[id[i][0]][id[i][1]][id[i][2]].setZRot(angle);
		}
	}
}

/**
Pauses the program for 1 millisecond
@author Evan Heetderks
*/
void delay() {
	clock_t start = clock();
	while (clock() - start < (long)1);
}

/**
Rotates a layer to its closest aligned position after an arbitrary rotation
@author Evan Heetderks
*/
void alignLayer() {
	int quarterRotations;
	int remainder;
	if (xRotation) {
		// Select an arbitrary cube from the layer and find out how many full quarter rotations it has made about the x axis
		quarterRotations = (cubes[selectedCubeIDs[0][0]][selectedCubeIDs[0][1]][selectedCubeIDs[0][2]].getXRot()/90) % 4;
		// Find out the remaining rotation amount in degrees past the number of full quarter rotations it has made
		remainder = cubes[selectedCubeIDs[0][0]][selectedCubeIDs[0][1]][selectedCubeIDs[0][2]].getXRot() % 90;

	}
	if (yRotation) {
		// Select an arbitrary cube from the layer and find out how many full quarter rotations it has made about the y axis
		quarterRotations = (cubes[selectedCubeIDs[0][0]][selectedCubeIDs[0][1]][selectedCubeIDs[0][2]].getYRot()/90) % 4;
		// Find out the remaining rotation amount in degrees past the number of full quarter rotations it has made
		remainder = cubes[selectedCubeIDs[0][0]][selectedCubeIDs[0][1]][selectedCubeIDs[0][2]].getYRot() % 90;

	}
	if (zRotation) {
		// Select an arbitrary cube from the layer and find out how many full quarter rotations it has made about the z axis
		quarterRotations = (cubes[selectedCubeIDs[0][0]][selectedCubeIDs[0][1]][selectedCubeIDs[0][2]].getZRot()/90) % 4;
		// Find out the remaining rotation amount in degrees past the number of full quarter rotations it has made
		remainder = cubes[selectedCubeIDs[0][0]][selectedCubeIDs[0][1]][selectedCubeIDs[0][2]].getZRot() % 90;
	}

	// Scale the rotation back to the closest positive point of alignment, pausing and displaying for each change in degree for visual effect
	if (remainder > 0) {
		if (remainder < 45) {
			while (remainder != 0) {
				remainder--;
				rotateCubes(-1, xRotation, yRotation, zRotation, selectedCubeIDs);
				displayFunc();
				delay();
			}
		}
		else {
			while (remainder != 90) {
				remainder++;
				rotateCubes(1, xRotation, yRotation, zRotation, selectedCubeIDs);
				displayFunc();
				delay();
			}
			quarterRotations++;

		}
	}
	if (remainder < 0) {
		if (remainder < -45) {
			while (remainder != -90) {
				remainder--;
				rotateCubes(-1, xRotation, yRotation, zRotation, selectedCubeIDs);
				displayFunc();
				delay();
			}
			quarterRotations--;

		}
		else {
			while (remainder != 0) {
				remainder++;
				rotateCubes(1, xRotation, yRotation, zRotation, selectedCubeIDs);
				displayFunc();
				delay();
			}

		}
	}

	// Update each cube in the layer based on the type and magnitude of rotation
	if (xRotation) {
		for (int i = 0; i < 9; i++) {
			cubes[selectedCubeIDs[i][0]][selectedCubeIDs[i][1]][selectedCubeIDs[i][2]].xRotUpdate(quarterRotations);
		}
	}

	if (yRotation) {
		for (int i = 0; i < 9; i++) {
			cubes[selectedCubeIDs[i][0]][selectedCubeIDs[i][1]][selectedCubeIDs[i][2]].yRotUpdate(quarterRotations);
		}
	}

	if (zRotation) {
		for (int i = 0; i < 9; i++) {
			cubes[selectedCubeIDs[i][0]][selectedCubeIDs[i][1]][selectedCubeIDs[i][2]].zRotUpdate(quarterRotations);
		}
	}
}

/**
Updates the selectedCubeIDs array to contain the x, y, and z IDs of a desired x, y, or z layer
@author Evan Heetderks
@param x: 1 an x layer is desired (otherwise 0)
@param y: 1 if an y layer is desired (otherwise 0)
@param z: 1 if an z layer is desired (otherwise 0)
@param number: the number of the desired layer (0, 1, or 2)
*/
void setLayerID(int x, int y, int z, int number) {
	int marker = 0;
	if (x == 1) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (cubes[i][j][k].getXLayer() == number) {
						selectedCubeIDs[marker][0] = i;
						selectedCubeIDs[marker][1] = j;
						selectedCubeIDs[marker][2] = k;
						marker++;
					}
				}
			}
		}
	}
	if (y == 1) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (cubes[i][j][k].getYLayer() == number) {
						selectedCubeIDs[marker][0] = i;
						selectedCubeIDs[marker][1] = j;
						selectedCubeIDs[marker][2] = k;
						marker++;
					}
				}
			}
		}
	}
	if (z == 1) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (cubes[i][j][k].getZLayer() == number) {
						selectedCubeIDs[marker][0] = i;
						selectedCubeIDs[marker][1] = j;
						selectedCubeIDs[marker][2] = k;
						marker++;
					}
				}
			}
		}
	}
}

/**
Selects a layer to rotate based on camera angle and mouse position
@author Evan Heetderks
*/
void selectLayer() {
	// Get GL matrix data
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble startX; GLdouble endX;
	GLdouble startY; GLdouble endY;
	GLdouble startZ; GLdouble endZ;

	// Get the x and y coordinates of the mouse on the screen
	GLdouble winX = (double)mouseX;
	GLdouble winY = viewport[3] - (double)mouseY;

	// Unproject the mouse coordinates to get the near depth and far depth world coordinates of the mouse
	gluUnProject(winX, winY, 0.0, modelViewMatrix, projectionMatrix, viewport, &startX, &startY, &startZ);
	gluUnProject(winX, winY, 1.0, modelViewMatrix, projectionMatrix, viewport, &endX, &endY, &endZ);

	// Set the cameraDirection vector
	cameraDirection.set(endX - startX, endY - startY, endZ - startZ);

	// Create an array that stores the scale values for the cameraDirection vector when it crosses the plane of each rubix cube face
	double scales[6];
	scales[0] = ((frontCenterPoint.subtract(cameraPosition)).dotProduct(frontNormal)) / cameraDirection.dotProduct(frontNormal);
	scales[1] = ((backCenterPoint.subtract(cameraPosition)).dotProduct(backNormal)) / cameraDirection.dotProduct(backNormal);
	scales[2] = ((topCenterPoint.subtract(cameraPosition)).dotProduct(topNormal)) / cameraDirection.dotProduct(topNormal);
	scales[3] = ((bottomCenterPoint.subtract(cameraPosition)).dotProduct(bottomNormal)) / cameraDirection.dotProduct(bottomNormal);
	scales[4] = ((leftCenterPoint.subtract(cameraPosition)).dotProduct(leftNormal)) / cameraDirection.dotProduct(leftNormal);
	scales[5] = ((rightCenterPoint.subtract(cameraPosition)).dotProduct(rightNormal)) / cameraDirection.dotProduct(rightNormal);

	// Vectors created from the camera position to the position on each rubix cube face plane that the cameraDirection vector intersects
	Vector frontFacePos = cameraDirection.scale(cameraPosition, scales[0]);
	Vector backFacePos = cameraDirection.scale(cameraPosition, scales[1]);
	Vector topFacePos = cameraDirection.scale(cameraPosition, scales[2]);
	Vector bottomFacePos = cameraDirection.scale(cameraPosition, scales[3]);
	Vector leftFacePos = cameraDirection.scale(cameraPosition, scales[4]);
	Vector rightFacePos = cameraDirection.scale(cameraPosition, scales[5]);

	// Array to hold the ID of the two faces that the cameraDirectionVector intersects
	int intersectedFaces[2];
	int marker = 0;
	
	// Determine if any of the scaled cameraDirection vectors for face plane actually intersects the face, if it does, add to the array
	if (frontFacePos.getXValue() > -3 &&
		frontFacePos.getXValue() < 3 &&
		frontFacePos.getYValue() > -3 &&
		frontFacePos.getYValue() < 3) {
		intersectedFaces[marker] = 0;
		marker++;
	}
	if (backFacePos.getXValue() > -3 &&
		backFacePos.getXValue() < 3 &&
		backFacePos.getYValue() > -3 &&
		backFacePos.getYValue() < 3) {
		intersectedFaces[marker] = 1;
		marker++;
	}
	if (topFacePos.getXValue() > -3 &&
		topFacePos.getXValue() < 3 &&
		topFacePos.getZValue() > -3 &&
		topFacePos.getZValue() < 3) {
		intersectedFaces[marker] = 2;
		marker++;
	}
	if (bottomFacePos.getXValue() > -3 &&
		bottomFacePos.getXValue() < 3 &&
		bottomFacePos.getZValue() > -3 &&
		bottomFacePos.getZValue() < 3) {
		intersectedFaces[marker] = 3;
		marker++;
	}
	if (leftFacePos.getYValue() > -3 &&
		leftFacePos.getYValue() < 3 &&
		leftFacePos.getZValue() > -3 &&
		leftFacePos.getZValue() < 3) {
		intersectedFaces[marker] = 4;
		marker++;
	}
	if (rightFacePos.getYValue() > -3 &&
		rightFacePos.getYValue() < 3 &&
		rightFacePos.getZValue() > -3 &&
		rightFacePos.getZValue() < 3) {
		intersectedFaces[marker] = 5;
		marker++;
	}

	// If the array was empty (no faces intersected and thus the cursor is going off into space), exit the function as no layer can  be selected
	if (marker != 2) {
		return;
	}
	
	// Notify system that a layer is being set
	layerBeingSet = true;
	
	// If determine which of the two intersected faces is closer to the camera (that one is the one being clicked on the screen)
	int selectedFace;
	if (scales[intersectedFaces[1]] > scales[intersectedFaces[0]]) {
		selectedFace = intersectedFaces[0];
	}
	else {
		selectedFace = intersectedFaces[1];
	}

	/*  The ID of the face the camera is facing
		0 - front face
		1 - back face
		2 - top face
		3 - bottom face
		4 - left face
		5 - right face
	*/
	int cameraFaceLocation;

	// Determine the closest face to the camera

	// If camera looking at front face
	if (cameraPosition.angleBetween(posYposZ) <= 90 &&
		cameraPosition.angleBetween(negYposZ) <= 90 &&
		cameraPosition.angleBetween(negXposZ) <= 90 &&
		cameraPosition.angleBetween(posXposZ) <= 90) {
		cameraFaceLocation = 0;
		rotationDirection = -1;
	}
		
	// If camera looking at back face
	else if (cameraPosition.angleBetween(posYnegZ) <= 90 &&
		cameraPosition.angleBetween(negYnegZ) <= 90 &&
		cameraPosition.angleBetween(negXnegZ) <= 90 &&
		cameraPosition.angleBetween(posXnegZ) <= 90) {
		cameraFaceLocation = 1;
		rotationDirection = 1;
	}
	
	// If camera looking at top face
	else if (cameraPosition.angleBetween(posYposZ) <= 90 &&
		cameraPosition.angleBetween(posXposY) <= 90 &&
		cameraPosition.angleBetween(posYnegZ) <= 90 &&
		cameraPosition.angleBetween(negXposY) <= 90) {
		cameraFaceLocation = 2;
		rotationDirection = -1;
	}
	
	// If camera looking at bottom face
	else if (cameraPosition.angleBetween(negYposZ) <= 90 &&
		cameraPosition.angleBetween(posXnegY) <= 90 &&
		cameraPosition.angleBetween(negYnegZ) <= 90 &&
		cameraPosition.angleBetween(negXnegY) <= 90) {
		cameraFaceLocation = 3;
		rotationDirection = 1;
	}
	
	// If camera is looking at left face
	else if (cameraPosition.angleBetween(negXposZ) <= 90 &&
		cameraPosition.angleBetween(negXnegY) <= 90 &&
		cameraPosition.angleBetween(negXnegZ) <= 90 &&
		cameraPosition.angleBetween(negXposY) <= 90) {
		cameraFaceLocation = 4;
		rotationDirection = 1;
	}

	// If camera is looking at right face
	else {
		cameraFaceLocation = 5;
		rotationDirection = -1;
	}

	// Get the IDs of the cubes in a layer bing clicked based on which face the camera is facing
	if (cameraFaceLocation == 0 || cameraFaceLocation == 1) {
		zRotation = true;
		if (selectedFace == 0) {
			setLayerID(0, 0, 1, 2);
		}
		else if (selectedFace == 1) {
			setLayerID(0, 0, 1, 0);
		}
		else if (selectedFace == 2) {
			if (topFacePos.getZValue() >= 1) {
				setLayerID(0, 0, 1, 2);
			}
			else if (topFacePos.getZValue() <= -1) {
				setLayerID(0, 0, 1, 0);
			}
			else {
				setLayerID(0, 0, 1, 1);
			}
		}
		else if (selectedFace == 3) {
			if (bottomFacePos.getZValue() >= 1) {
				setLayerID(0, 0, 1, 2);
			}
			else if (bottomFacePos.getZValue() <= -1) {
				setLayerID(0, 0, 1, 0);
			}
			else {
				setLayerID(0, 0, 1, 1);
			}
		}
		else if (selectedFace == 4) {
			if (leftFacePos.getZValue() >= 1) {
				setLayerID(0, 0, 1, 2);
			}
			else if (leftFacePos.getZValue() <= -1) {
				setLayerID(0, 0, 1, 0);
			}
			else {
				setLayerID(0, 0, 1, 1);
			}
		}
		else {
			if (rightFacePos.getZValue() >= 1) {
				setLayerID(0, 0, 1, 2);
			}
			else if (rightFacePos.getZValue() <= -1) {
				setLayerID(0, 0, 1, 0);
			}
			else {
				setLayerID(0, 0, 1, 1);
			}
		}
	}
	else if (cameraFaceLocation == 2 || cameraFaceLocation == 3) {
		yRotation = true;
		if (selectedFace == 0) {
			if (frontFacePos.getYValue() >= 1) {
				setLayerID(0, 1, 0, 2);
			}
			else if (frontFacePos.getYValue() <= -1) {
				setLayerID(0, 1, 0, 0);
			}
			else {
				setLayerID(0, 1, 0, 1);
			}
		}
		else if (selectedFace == 1) {
			if (backFacePos.getYValue() >= 1) {
				setLayerID(0, 1, 0, 2);
			}
			else if (backFacePos.getYValue() <= -1) {
				setLayerID(0, 1, 0, 0);
			}
			else {
				setLayerID(0, 1, 0, 1);
			}
		}
		else if (selectedFace == 2) {
			setLayerID(0, 1, 0, 2);
		}
		else if (selectedFace == 3) {
			setLayerID(0, 1, 0, 0);
		}
		else if (selectedFace == 4) {
			if (leftFacePos.getYValue() >= 1) {
				setLayerID(0, 1, 0, 2);
			}
			else if (leftFacePos.getYValue() <= -1) {
				setLayerID(0, 1, 0, 0);
			}
			else {
				setLayerID(0, 1, 0, 1);
			}
		}
		else {
			if (rightFacePos.getYValue() >= 1) {
				setLayerID(0, 1, 0, 2);
			}
			else if (rightFacePos.getYValue() <= -1) {
				setLayerID(0, 1, 0, 0);
			}
			else {
				setLayerID(0, 1, 0, 1);
			}
		}
	}
	else {
		xRotation = true;
		if (selectedFace == 0) {
			if (frontFacePos.getXValue() >= 1) {
				setLayerID(1, 0, 0, 2);
			}
			else if (frontFacePos.getXValue() <= -1) {
				setLayerID(1, 0, 0, 0);
			}
			else {
				setLayerID(1, 0, 0, 1);
			}
		}
		else if (selectedFace == 1) {
			if (backFacePos.getXValue() >= 1) {
				setLayerID(1, 0, 0, 2);
			}
			else if (backFacePos.getXValue() <= -1) {
				setLayerID(1, 0, 0, 0);
			}
			else {
				setLayerID(1, 0, 0, 1);
			}
		}
		else if (selectedFace == 2) {
			if (topFacePos.getXValue() >= 1) {
				setLayerID(1, 0, 0, 2);
			}
			else if (topFacePos.getXValue() <= -1) {
				setLayerID(1, 0, 0, 0);
			}
			else {
				setLayerID(1, 0, 0, 1);
			}
		}
		else if (selectedFace == 3) {
			if (bottomFacePos.getXValue() >= 1) {
				setLayerID(1, 0, 0, 2);
			}
			else if (bottomFacePos.getXValue() <= -1) {
				setLayerID(1, 0, 0, 0);
			}
			else {
				setLayerID(1, 0, 0, 1);
			}
		}
		else if (selectedFace == 4) {
			setLayerID(1, 0, 0, 0);
		}
		else {
			setLayerID(1, 0, 0, 2);
		}
	}

}

/**
Gets 
@author Evan Heetderks
@param button: the mouse button concerned
@param state: whether the button is being pressed
@param x: the x location of the mouse on the screen
@param y: the y location of the mouse on the screen
*/
void mouseFunc(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		mouseState = state;
		mouseButton = button;
		mouseX = x;
		mouseY = y;
		
		if (mouseButton == GLUT_RIGHT_BUTTON) {
			selectLayer();
		}
	}
	else {
		mouseState = 0;
		if (mouseButton == GLUT_RIGHT_BUTTON && layerBeingSet) {
			alignLayer();
		}
		layerBeingSet = false;
		xRotation = false;
		yRotation = false;
		zRotation = false;
	}
}

/**
Adjust camera angle when the left button is clicked and draged, rotates a selected layer when right button is clicked and draged
@author Evan Heetderks
@param x: the x location of the mouse on the screen
@param y: the y location of the mouse on the screen
*/
void motionFunc(int x, int y) {
	if (mouseState == GLUT_DOWN) {
		// Modify the camera position if the right button is being clicked
		if (mouseButton == GLUT_LEFT_BUTTON) {
			// Increase or decrease theta and phi camera rotation angles, keeping them in the range of [0, 2PI)
			if (phi + ((y - mouseY) / 200) >= (2*(float)PI)) {
				phi = ((float)(y - mouseY) / 200) - (2*(float)PI);
			}
			else if (phi + ((y - mouseY) / 200) < 0) {
				phi = (2*(float)PI) + phi + ((float)(y - mouseY) / 200);
			}
			else {
				phi = phi + ((float)(y - mouseY) / 200);
			}

			
			if (phi >= (3 * (float)PI / 2) || phi < ((float)PI / 2)) {
				if (theta - ((x - mouseX) / 200) >= (2 * (float)PI)) {
					theta = -((float)(x - mouseX) / 200) - (2 * (float)PI);
				}
				else if (theta - ((x - mouseX) / 200) < 0) {
					theta = (2 * (float)PI) + theta - ((float)(x - mouseX) / 200);
				}
				else {
					theta = theta - ((float)(x - mouseX) / 200);
				}
			}
			else {
				if (theta + ((x - mouseX) / 200) >= (2 * (float)PI)) {
					theta = ((float)(x - mouseX) / 200) - (2 * (float)PI);
				}
				else if (theta + ((x - mouseX) / 200) < 0) {
					theta = (2 * (float)PI) + theta + ((float)(x - mouseX) / 200);
				}
				else {
						theta = theta + ((float)(x - mouseX) / 200);
				}
			}

			// Set the new camera position based on the new camera rotation angles
			cameraPosition.set(cameraRadius * sin(theta) * cos(phi), cameraRadius * sin(phi), cameraRadius * cos(theta) * cos(phi));
		}
		// Rotate a layer if the right button is being clicked (if the rubix cube on the screen is also being clicked
		if (mouseButton == GLUT_RIGHT_BUTTON) {
			rotateCubes(rotationDirection * ((x - mouseX) / 2), xRotation, yRotation, zRotation, selectedCubeIDs);
		}
	}
	mouseX = x;
	mouseY = y;
}

/**
Routine for adjusting camera perspective if the window size is modifed
@author Evan Heetderks
@param width: the new width of the screen
@param height: the new height of the screen
*/
void reshapeFunc(int width, int height) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (height == 0) {
		height = 1;
	}
	double ratio = 1.0f * width / height;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the clipping volume
	gluPerspective(80, ratio, 1, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 0, 30, 0, 0, 10, 0.0f, 1.0f, 0.0f);
	// Set camera position and orientation
	if (phi >= (3 * PI / 2) || phi < (PI / 2)) {
		// Camera rightside up if 3pi/2 <= phi < pi/2
		gluLookAt(cameraPosition.getXValue(), cameraPosition.getYValue(), cameraPosition.getZValue(), 0, 0, 0, 0.0f, 1.0f, 0.0f);
	}
	else {
		// Camera upside down if pi/2 <= phi < 3pi/2
		gluLookAt(cameraPosition.getXValue(), cameraPosition.getYValue(), cameraPosition.getZValue(), 0, 0, 0, 0.0f, -1.0f, 0.0f);
	}
}

/**
Where all main functions are implemented
@author Evan Heetderks
@param argc
@param argv
*/
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1000, 500);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Rubix cube");
	initialize();
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutIdleFunc(displayFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutMainLoop();
}