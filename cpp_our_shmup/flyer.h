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
};

