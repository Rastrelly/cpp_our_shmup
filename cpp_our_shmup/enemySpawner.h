#pragma once

#include "flyer.h"

class enemySpawner
{
public:

	int gameLevel;

	bool iterationLocked;
	float iteration;
	float timer;
	std::vector<flyer*> *pFlyers;
	OGLManager * pMgr;
	int * pScore;

	std::vector<unsigned int> texIndices;
	std::vector<unsigned int> groundTextures;
	int cGroundTex;

	enemySpawner(std::vector<flyer*> *flyersContainer,
		OGLManager * mgr, int * scoreRef);

	int addIndexedTexture(unsigned int tex);
	void iterate(float dt);
	void runFiring();
	void makeAttack(int attType, int bId, glm::vec2 csPos, glm::vec2 tsPos);
	void spawnEnemy(int eType, glm::vec2 spos, glm::vec2 tpos);
	void spawnEnemyWave(int eType, int waveType);
	void addGroundTexture(unsigned int gt);
	void checkBulletCollisions();
	bool bossExists();
	void nextlevel() { gameLevel++; };
	void resetLevel() { gameLevel = 1; };
	
};

