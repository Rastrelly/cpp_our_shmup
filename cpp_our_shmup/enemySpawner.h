#pragma once

#include "flyer.h"

class enemySpawner
{
public:
	bool iterationLocked;
	float iteration;
	float timer;
	std::vector<flyer*> *pFlyers;
	OGLManager * pMgr;

	std::vector<unsigned int> texIndices;

	enemySpawner(std::vector<flyer*> *flyersContainer, OGLManager * mgr);

	int addIndexedTexture(unsigned int tex);
	void iterate(float dt);
	void spawnEnemy(int eType);

	
};

