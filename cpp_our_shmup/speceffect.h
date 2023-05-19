#pragma once

#include "ourGraphics.h"

class speceffect
{
  private:
	int cframe;
	float myTimer;
  public:
	glm::vec3 position;
	float life;
	float fps;
	int nframes, ncols, nrows;
	OGLManager * glManager;
	unsigned int tex;

	void processEffect(float dt);
	void drawSpecEffect(float dt);
	speceffect(OGLManager * vglm, int vc, float vLife, glm::vec3 vpos, unsigned int vtex);

};

