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
	gameLevel = 1;
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

	if (bossExists()) iterationLocked = true;

	if (iterationLocked) return; //return gate to not spawn if
	//iteration locked

	if ((int)iteration % 2 == 0) //dangerous spot! can cause
	//we can get to int limit early
	{
		if (rand() % 101 < 51 + round(iteration/250.0f))
		{
			spawnEnemy(1,glm::vec2((float)(rand() % 801 - 400),350.0f), glm::vec2(0.0f));
			iterationLocked = true;
		} else if (rand() % 101 < 51 + round(iteration / 250.0f))
		{
			spawnEnemy(4,glm::vec2((float)(rand() % 721 - 360), 350.0f), glm::vec2(0.0f));
			iterationLocked = true;
		}
		else if (rand() % 101 < 51 + round(iteration / 250.0f))
		{
			if (rand()%2==1) spawnEnemyWave(1,rand()%3);
			else spawnEnemyWave(4, rand() % 3);
			iterationLocked = true;
		}
	}

	if ((int)iteration % 20 == 0 && iteration > 250)
	{
		iterationLocked = true;
	    spawnEnemyWave(7, rand() % 3);
	}

	if ((int)iteration % 9 == 0)
	{
		if (rand() % 101 < 41 + round(iteration / 250.0f)) //eneme
		{
			spawnEnemy(7, glm::vec2((float)(rand() % 721 - 360), 350.0f), glm::vec2(0.0f));
			iterationLocked = true;
		}
	}

	if ((int)iteration % 4 == 0)
	{
		int hpc = 66 - round(iteration / 250.0f);
		if (hpc < 10) hpc = 10;
		if (rand() % 101 < hpc) //hp
		{
			spawnEnemy(5, glm::vec2((float)(rand() % 721 - 360), 350.0f), glm::vec2(0.0f));
			iterationLocked = true;
		}
	}

	if ((int)iteration % 20 == 0)
	{
		int hpc = 35 - 2*round(iteration / 250.0f);
		if (hpc < 2) hpc = 2;
		
		if (rand() % 101 < hpc) //gun
		{
			spawnEnemy(8, glm::vec2((float)(rand() % 721 - 360), 350.0f), glm::vec2(0.0f));
			iterationLocked = true;
		}
	}

	if ((int)iteration % 250 == 0)
	{
		iterationLocked = true;
		spawnEnemy(6, glm::vec2(0.0f, 350.0f), glm::vec2(0.0f));
		cGroundTex = rand() % groundTextures.size();
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
	if (enemyId == 3) return 2; //enemy bullet
	if (enemyId == 4) return 3; //enemy ship 2
	if (enemyId == 5) return 4; //hp powerup
	if (enemyId == 6) return 5; //boss 1
	if (enemyId == 7) return 6; //enemy ship 3
	if (enemyId == 8) return 7; //gun powerup
}

void enemySpawner::spawnEnemy(int eType, glm::vec2 spos, glm::vec2 tpos)
{
	if (eType != -1)
	{
	    glm::vec2 spawnPos(0.0f);
		int aR = 1, aC = 1;
		if (eType == 1 || eType == 4 || eType == 5 || eType == 6 || eType == 7 || eType == 8)
		{
			spawnPos = spos;
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
		//std::cout << "Enemy " << eType << " spawned (iter =="<< iteration <<")!\n";
	}
		
}

float getWaveCY(float cX, int waveType)
{
	if (waveType == 0)
	{
		return 350.0f;
	}
	if (waveType == 1)
	{
		float cxT = (cX - 40.0f) / 800.0f;
		float cyT = pow(cxT, 2);
		return 350.0f + (100.0f*cyT - 25.0f);
	}
	if (waveType == 2)
	{
		float cxT = (cX - 40.0f) / 800.0f;
		float cyT = cos(10*3.14f*cxT);
		return 350.0f + (100.0f*cyT - 25.0f);
	}
}

void enemySpawner::spawnEnemyWave(int eType, int waveType)
{
	float startX = 40.0f;
	float endX = 760.0f;
	float dX = 80.0f;
	float cX = startX;
	float cY = 0.0f;
	for (cX = startX; cX <= endX; cX += dX)
	{
		cY = getWaveCY(cX,waveType);
		spawnEnemy(eType,glm::vec2(cX - 380.0f,cY),glm::vec2(0.0f));
	}
}

void enemySpawner::makeAttack(int attType, int bId, glm::vec2 csPos, glm::vec2 tsPos)
{
	if (attType == 0)
	{
		spawnEnemy(
			bId,
			csPos,
			tsPos
		);
	}


	if (attType == 1)
	{
		spawnEnemy(
			bId,
			csPos,
			tsPos
		);
		spawnEnemy(
			bId,
			csPos + glm::vec2(-20.0f, 0.0f),
			tsPos + glm::vec2(-20.0f, 0.0f)
		);
		spawnEnemy(
			bId,
			csPos + glm::vec2(20.0f, 0.0f),
			tsPos + glm::vec2(20.0f, 0.0f)
		);
	}


	if (attType == 2)
	{
		float iAng = 0.0f;

		for (iAng = 0.0f; iAng < 360.0f; iAng += 30)
		{
			tsPos.x = csPos.x + 100.0f * sin(iAng*3.14f / 180.0f);
			tsPos.y = csPos.y + 100.0f * cos(iAng*3.14f / 180.0f);
			spawnEnemy(
				bId,
				csPos,
				tsPos
			);
		}
	}
}


void enemySpawner::runFiring()
{
	int l = pFlyers->size();
	for (int i = 0; i < l; i++)
	{
		if ((*pFlyers)[i]->getCanShoot())
		{
			if ((*pFlyers)[i]->getMyType() == 6)
			{
				//remove can shoot flag
				(*pFlyers)[i]->setCanShoot(false);
				//spawn a bullet

				glm::vec2 csPos = (*pFlyers)[i]->getPos();
				glm::vec2 tsPos(0.0f);

				if (rand()%2==0)
				  tsPos = (*pFlyers)[i]->getTargetPos();
				else
				  tsPos = csPos - glm::vec2(0.0f, 100.0f);

				makeAttack(1, (*pFlyers)[i]->getBulletId(),csPos, tsPos);
				
				//report on firing
				printf("Ship %d trying to shoot: p = %f, %f; t = %f, %f\n",
					i, csPos.x, csPos.y, tsPos.x, tsPos.y);
			}
			else if ((*pFlyers)[i]->getMyType() == 7)
			{
				//remove can shoot flag
				(*pFlyers)[i]->setCanShoot(false);
				//spawn a bullet

				glm::vec2 csPos = (*pFlyers)[i]->getPos();
				glm::vec2 tsPos(0.0f);
				makeAttack(2, (*pFlyers)[i]->getBulletId(), csPos, tsPos);
			}
			else if ((*pFlyers)[i]->getMyType() == 0)
			{
				//remove can shoot flag
				(*pFlyers)[i]->setCanShoot(false);
				//spawn a bullet
				int gl = (*pFlyers)[i]->getGunLevel();
				float gd = 16.0f;
				float gw = gd * (float)gl;
				float gx = gw / 2.0f;

				for (int jj = 0; jj < gl; jj++)
				{
					glm::vec2 cShift(-gx + gd * jj, 0.0f);
					glm::vec2 csPos = (*pFlyers)[i]->getPos() + cShift;
					glm::vec2 tsPos = csPos + cShift;
					makeAttack(0, (*pFlyers)[i]->getBulletId(), csPos, tsPos);
				}
				
			}
			else
			{
				//remove can shoot flag
				(*pFlyers)[i]->setCanShoot(false);
				//spawn a bullet

				glm::vec2 csPos = (*pFlyers)[i]->getPos();
				glm::vec2 tsPos = (*pFlyers)[i]->getTargetPos();
				makeAttack(0, (*pFlyers)[i]->getBulletId(), csPos, tsPos);

				//report on firing
				printf("Ship %d trying to shoot: p = %f, %f; t = %f, %f\n",
					i, csPos.x, csPos.y, tsPos.x, tsPos.y);
			}

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
	{
		if ((*pFlyers)[i]->getDed()) continue;
		if ((*pFlyers)[i]->getIsBullet()) //check each flyer that is a bullet
		{
			float cr1 = (*pFlyers)[i]->getCollisionRadius();
			for (int j = 0; j < l; j++)       //for each flyer that isn't a bullet
			{
				if ((*pFlyers)[i]->getDed()) break;
				if ((!(*pFlyers)[j]->getIsBullet()) && (!(*pFlyers)[j]->getIsPowerUp()) && (i != j) && (!(*pFlyers)[i]->getDed()) && ((*pFlyers)[j]->getTeam() != (*pFlyers)[i]->getTeam()))
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
		if ((*pFlyers)[i]->getIsPowerUp())
		{
			float od = getDist((*pFlyers)[i]->getPos(), (*pFlyers)[0]->getPos());
			if (od < 32.0f)
			{
				if ((*pFlyers)[i]->getPowerupType() == 1) (*pFlyers)[0]->increaseHp();
				if ((*pFlyers)[i]->getPowerupType() == 2) (*pFlyers)[0]->upgradeGun();
				(*pFlyers)[i]->setDed(true);
			}
		}
	}
}


bool enemySpawner::bossExists()
{
	int l = pFlyers->size();

	for (int i = 0; i < l; i++)
	{
		if ((*pFlyers)[i]->getDed()) continue;
		if ((*pFlyers)[i]->getIsBoss()) return true;
	}
	return false;
}


void enemySpawner::addGroundTexture(unsigned int gt)
{
	groundTextures.push_back(gt);
}