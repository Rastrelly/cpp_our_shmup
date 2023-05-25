#include "flyer.h"

flyer::flyer(int vmyType, glm::vec2 vpos,
	OGLManager *vmyManager, float vr,
	unsigned int vmyTex, int vanimR, int vanimC,
	glm::vec2 * vDirAxis)
{
	myType = vmyType;
	pos = vpos;
	r = vr;
	myTex = vmyTex;
	animR = vanimR;
	animC = vanimC;
	myManager = vmyManager;
	fps = 15.0f;
	nframes = animR * animC;
	dirAxis = vDirAxis;

	xmin = -400.0f;
	xmax = 400.0f;
	ymin = -300.0f;
	ymax = 300.0f;

	if (myType == 0)
	{
		speed = 100.0f;
	}
}

void flyer::processInternals(float dt, glm::vec2 dirAxR)
{
	//animation controller
	myTimer += dt;
	if (myTimer > (1.0f / fps))
	{
		myTimer = 0.0f;
		cframe++;
		if (cframe > nframes - 1) cframe = 0;
	}
	//motion
	if (myType == 0)
	{

		if (glm::length(dirAxR)!=0.0f)
		{
			dir = glm::normalize(dirAxR);
		}
		else
		{
			dir = dirAxR;
		}
		glm::vec2 moveVec(0.0f);
		moveVec = dir * speed * dt;

		pos += moveVec;

		if (pos.x < xmin) pos.x = xmin;
		if (pos.x > xmax) pos.x = xmax;
		if (pos.y < ymin) pos.y = ymin;
		if (pos.y > ymax) pos.y = ymax;
		printf("Moveinp: x = %f, y = %f, dx = %f, dy = %f, spd = %f, dt = %f\n", pos.x, pos.y, dir.x, dir.y, speed, dt);
	}

	//render
	drawMe();
}


void flyer::drawMe()
{
	glm::vec3 npos(pos.x,pos.y,0.0f);
	drawSprites(myManager->getShader(0), npos, glm::vec3(r), glm::vec3(1.0f), myTex, true, 3, 3, cframe);
}