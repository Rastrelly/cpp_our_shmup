#include "enemySpawner.h"

enemySpawner::enemySpawner(std::vector<flyer*> *flyersContainer,
	OGLManager * mgr)
{
	pFlyers = flyersContainer;
	pMgr = mgr;
	timer = 0.0f;
	iteration = 0.0f;
	iterationLocked = true;
}

void enemySpawner::iterate(float dt)
{
	timer += dt;
	if (timer >= 0.5f)
	{
		timer = 0.0f;
		iteration++;
		iterationLocked = false;
		std::cout << "Spawner iteration: " << iteration << std::endl;
	}

	if (iterationLocked) return; //return gate to not spawn if
	//iteration locked

	if ((int)iteration % 2 == 0) //dangerous spot! can cause
	//we can get to int limit early
	{
		if (rand() % 101 < 51)
		{
			spawnEnemy(1);
			std::cout << "Enemy spawned!\n";
			iterationLocked = true;
		}
	}
}

int enemySpawner::addIndexedTexture(unsigned int tex)
{
	texIndices.push_back(tex);
	return texIndices.size() - 1;
}

int texIndexByEnemyId(int enemyId)
{
	if (enemyId == 1) return 0;
}

void enemySpawner::spawnEnemy(int eType)
{
	glm::vec2 spawnPos(0.0f);
	if (eType == 1)
	{
		int spX = rand() % 801 - 400;
		int spY = 350;
		spawnPos.x = (float)spX;
		spawnPos.y = (float)spY;
	}
	pFlyers->push_back(
		new flyer(eType, spawnPos, pMgr, 25.0f, texIndices[texIndexByEnemyId(eType)], 2, 2)
	);
}