#include "speceffect.h"

void speceffect::processEffect(float dt)
{
	if (life > 0)
	{
		life -= dt;
		drawSpecEffect(dt);
		if (life <= 0) printf("Effect gone!\n");
	}
}


void speceffect::drawSpecEffect(float dt)
{
	myTimer += dt;
	if (myTimer > (1.0f / fps))
	{
		myTimer = 0.0f;
		cframe++;
		if (cframe > nframes - 1) cframe = 0;
	}
	drawSprites(glManager->getShader(0), glm::vec3(position.x,position.y,0.0f), glm::vec3(30.0f), glm::vec3(1.0f), tex, true, ncols, nrows, cframe);
}

speceffect::speceffect(OGLManager * vglm, int vc, float vLife, glm::vec3 vpos, unsigned int vtex)
{
	life = vLife;
	glManager = vglm;
	ncols = vc;
	nrows = vc;
	nframes = vc * vc;
	position = vpos;
	tex = vtex;
	cframe = 0;
	myTimer = 0;
	fps = 8.0f;
}