#include "enemySpawner.h"

enemySpawner::enemySpawner(std::vector<flyer*> *flyersContainer,
	OGLManager * mgr, int * scoreRef)
{
	pFlyers = flyersContainer;
	pMgr = mgr;
	timer = 0.0f;
	iteration = 0.0f;
	iterationLocked = true;
	pScore = scoreRef;
}

void enemySpawner::iterate(float dt)
{
	timer += dt;
	if (timer >= 0.5f)
	{
		timer = 0.0f;
		iteration++;
		iterationLocked = false;
		//std::cout << "Spawner iteration: " << iteration << std::endl;
	}

	if (iterationLocked) return; //return gate to not spawn if
	//iteration locked

	if ((int)iteration % 2 == 0) //dangerous spot! can cause
	//we can get to int limit early
	{
		if (rand() % 101 < 51)
		{
			spawnEnemy(1,glm::vec2(0.0f), glm::vec2(0.0f));
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
	if (enemyId == 1) return 0; //enemy ship 1
	if (enemyId == 2) return 1; //plr bullet
	if (enemyId == 3) return 1; //enemy bullet
}

void enemySpawner::spawnEnemy(int eType, glm::vec2 spos, glm::vec2 tpos)
{
	if (eType != -1)
	{
	    glm::vec2 spawnPos(0.0f);
		int aR = 1, aC = 1;
		if (eType == 1)
		{
			int spX = rand() % 801 - 400;
			int spY = 350;
			spawnPos.x = (float)spX;
			spawnPos.y = (float)spY;
			aR = 2;
			aC = 2;
		}
		if (eType == 2 || eType == 3)
		{
			spawnPos = spos;
		};

		pFlyers->push_back(
			new flyer(eType, spawnPos, tpos, pMgr, 25.0f, texIndices[texIndexByEnemyId(eType)], aR, aC)
		);
		std::cout << "Enemy " << eType << " spawned!\n";
	}
}


void enemySpawner::runFiring()
{
	int l = pFlyers->size();
	for (int i = 0; i < l; i++)
	{
		if ((*pFlyers)[i]->getCanShoot())
		{
			//remove can shoot flag
			(*pFlyers)[i]->setCanShoot(false);
			//spawn a bullet

			glm::vec2 csPos = (*pFlyers)[i]->getPos();
			glm::vec2 tsPos = (*pFlyers)[i]->getTargetPos();
			spawnEnemy(
				(*pFlyers)[i]->getBulletId(),
				csPos, 
				tsPos
				);
			//report on firing
			printf("Ship %d trying to shoot: p = %f, %f; t = %f, %f\n",
				i, csPos.x, csPos.y, tsPos.x, tsPos.y);

		}
	}
}

float getDist(glm::vec2 p1, glm::vec2 p2)
{
	return glm::length(p2 - p1);
}

void enemySpawner::checkBulletCollisions()
{
	int l = pFlyers->size();

	for (int i = 0; i < l; i++)
		if ((*pFlyers)[i]->getIsBullet()) //check each flyer that is a bullet
		{
			float cr1 = (*pFlyers)[i]->getCollisionRadius();
			for (int j = 0; j < l; j++)       //for each flyer that isn't a bullet
				if ((!(*pFlyers)[j]->getIsBullet()) && (i != j) && (!(*pFlyers)[i]->getDed()) && ((*pFlyers)[j]->getTeam() != (*pFlyers)[i]->getTeam()))
				{
					float cr2 = (*pFlyers)[j]->getCollisionRadius();
					float cd = cr1 + cr2;
					float od = getDist((*pFlyers)[j]->getPos(), (*pFlyers)[i]->getPos());
					if (od <= cd)
					{
						(*pFlyers)[j]->applyDamage((*pFlyers)[i]->getDamage());
						(*pFlyers)[i]->setDed(true);
					}
				}
		}
}
