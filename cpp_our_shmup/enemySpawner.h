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
	int * pScore;

	std::vector<unsigned int> texIndices;

	enemySpawner(std::vector<flyer*> *flyersContainer,
		OGLManager * mgr, int * scoreRef);

	int addIndexedTexture(unsigned int tex);
	void iterate(float dt);
	void runFiring();
	void spawnEnemy(int eType, glm::vec2 spos, glm::vec2 tpos);
	void checkBulletCollisions();

	
};

