#pragma once

#include "ourGraphics.h"

class flyer
{
private:
	glm::vec2 pos;      //object coords
	glm::vec2 dir;      //object direction
	float speed;        //object speed (scalar)
	float r;            //object radius
	int myType;         //object type
	unsigned int myTex; //texture
	int animR, animC;   //animation rows, columns
	//working values
	int cframe;
	float myTimer;
	float fps;
	int nframes;

	float xmin, xmax, ymin, ymax;

	OGLManager *myManager;
	glm::vec2 *dirAxis;
public:
	flyer(int vmyType, glm::vec2 vpos, 
		OGLManager *vmyManager, float vr, 
		unsigned int vmyTex, int vanimR, int vanimC, 
		glm::vec2 * vDirAxis);
	void processInternals(float dt, glm::vec2 dirAxR);
	void drawMe();
};

