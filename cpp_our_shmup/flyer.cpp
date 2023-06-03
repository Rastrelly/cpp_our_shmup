#include "flyer.h"

flyer::flyer(int vmyType, glm::vec2 vpos, glm::vec2 tpos,
	OGLManager *vmyManager, float vr,
	unsigned int vmyTex, int vanimR, int vanimC)
{

	myType = vmyType;
	pos = vpos;
	targetPos = tpos;
	r = vr;
	myTex = vmyTex;
	animR = vanimR;
	animC = vanimC;
	myManager = vmyManager;
	canShoot = false;
	fps = 15.0f;
	nframes = animR * animC;

	needExplode = false;

	scoreValue = 0;

	xmin = -400.0f;
	xmax = 400.0f;
	ymin = -300.0f;
	ymax = 300.0f;

	reloadTimer = 0.0f;

	diesOob = true;
	ded = false;

	isBullet = false;
	isPowerUp = false;

	team = 1;

	inCounter = 0.0f;

	//set up ship/bullet specific properties
	if (myType == 0) //plr ship
	{
		speed = 350.0f;
		diesOob = false;
		reloadTime = 0.2f;
		reloadTimer = reloadTime;
		bulletId = 2;
		hp = 3.0f;
		team = 0;
		collisionR = 0.2f * vr;
		gunLevel = 1;
	}

	if (myType == 1) //en 1
	{
		speed = 400.0f;
		reloadTime = 1.2f;
		reloadTimer = reloadTime;
		bulletId = 3;
		hp = 1.0f;
		scoreValue = 100;
		myTrajectory = 0;
		collisionR = 1.1f * vr;
	}

	if (myType == 2) //plr bullet
	{
		speed = 650.0f;
		reloadTime = 0.0f;
		reloadTimer = reloadTime;
		bulletId = -1;
		dir = glm::vec2(0.0f,1.0f);
		r = 8.0f;
		dmg = 1.0f;
		isBullet = true;
		team = 0;
		myTrajectory = 0;
		collisionR = 1.1f * r;
	}

	if (myType == 3) //en bullet
	{
		speed = 400.0f;
		reloadTime = 0.0f;
		reloadTimer = reloadTime;
		bulletId = -1;
		dir = glm::normalize(tpos-pos);
		r = 8.0f;
		dmg = 1.0f;
		isBullet = true;
		myTrajectory = 0;
		collisionR = 0.8f * r;
	}

	if (myType == 4) //en 2
	{
		speed = 300.0f;
		reloadTime = 1.0f;
		reloadTimer = reloadTime;
		bulletId = 3;
		hp = 1.0f;
		scoreValue = 200;
		myTrajectory = 1;
		trajA = 50.0f;
		trajB = 0.2f;
		collisionR = 1.1f * vr;
	}

	if (myType == 5) //hp powerup
	{
		speed = 200.0f;
		reloadTime = 0.0f;
		reloadTimer = reloadTime;
		bulletId = -1;
		dir = glm::normalize(tpos - pos);
		r = 16.0f;
		collisionR = 1.1f * r;
		dmg = 0.0f;
		isBullet = false;
		isPowerUp = true;
		myTrajectory = 0;
		team = 0;
		powerUpType = 1;
	}

	if (myType == 6) //boss 1
	{
		speed = 200.0f;
		reloadTime = 0.6f;
		reloadTimer = reloadTime;
		bulletId = 3;
		dir = glm::normalize(tpos - pos);
		r = 64.0f;
		dmg = 0.0f;
		myTrajectory = 0;
		collisionR = 1.0f * r;
		hp = 40;
		isBoss = true;
		scoreValue = 1000;
	}

	if (myType == 7) //en 3
	{
		speed = 200.0f;
		reloadTime = 0.9f;
		reloadTimer = reloadTime;
		bulletId = 3;
		hp = 5.0f;
		r = 48.0;
		scoreValue = 350;
		myTrajectory = 0;
		collisionR = 1.1f * r;
	}

	if (myType == 8) //gun powerup
	{
		speed = 200.0f;
		reloadTime = 0.0f;
		reloadTimer = reloadTime;
		bulletId = -1;
		dir = glm::normalize(tpos - pos);
		r = 16.0f;
		collisionR = 1.1f * r;
		dmg = 0.0f;
		isBullet = false;
		isPowerUp = true;
		myTrajectory = 0;
		team = 0;
		powerUpType = 2;
	}
}

void flyer::processInternals(float dt, glm::vec2 dirAxR)
{
	//internal value counter
	inCounter += dt;

	//posthittimer
	hideSprite = false;
	if (postHitTimer > 0)
	{
		postHitTimer -= dt;
		if ((int)(round(postHitTimer * 10)) % 2 == 0) hideSprite = true;
	}

	//animation controller
	myTimer += dt;
	if (myTimer > (1.0f / fps))
	{
		myTimer = 0.0f;
		cframe++;
		if (cframe > nframes - 1) cframe = 0;
	}

	//update target pos
	if (myType != 0 && bulletId != -1)
	{
		targetPos = plrPos;
	}

	//shooting
	reloadTimer -= dt;
	if (reloadTimer <= 0)
	{
		reloadTimer = reloadTime;
		if (myType == 0 && usrShootCall) //user wants to shoot
		{
			canShoot = true;
		}
		if (myType != 0 && bulletId != -1) //enemies with assigned bullet type can shoot
		{
			canShoot = true;
		}
	}

	//motion

	glm::vec2 moveVec(0.0f);

	if (myType == 0) //user ship
	{

		if (glm::length(dirAxR)!=0.0f)
		{
			dir = glm::normalize(dirAxR);
		}
		else
		{
			dir = dirAxR;
		}
		
		moveVec = dir * speed * dt;

		pos += moveVec;

		if (pos.x < xmin) pos.x = xmin;
		if (pos.x > xmax) pos.x = xmax;
		if (pos.y < ymin) pos.y = ymin;
		if (pos.y > ymax) pos.y = ymax;
		//printf("Moveinp: x = %f, y = %f, dx = %f, dy = %f, spd = %f, dt = %f\n", pos.x, pos.y, dir.x, dir.y, speed, dt);
	}

	if (myType == 1 || myType == 5 || myType == 7 || myType == 8) //enemy 1 or powerup
	{
		dir = glm::vec2(0.0f,-1.0f);
		moveVec = dir * speed * dt;
		pos += moveVec;		
	}

	if (myType == 2) //player bullet
	{
		moveVec = dir * speed * dt;
		pos += moveVec;
	}

	if (myType == 3) //enemy bullet
	{
		moveVec = dir * speed * dt;
		pos += moveVec;
	}

	if (myType == 4) //enemy 2
	{
		dir = glm::vec2(0.0f, -1.0f);
		float dx = 0.0025f * trajA * sin(10.0f * trajB * inCounter);
		moveVec = (dir * speed * dt) + glm::vec2(dx,0.0f);
		pos += moveVec;
	}

	if (myType == 6) //boss 1
	{
		
		if (pos.y > 180.0f)
		{
			dir = glm::vec2(0.0f, -1.0f);			
		}
		else
		{
			if (abs(targetPos.x - pos.x) > 10.0f)
			{
				if (targetPos.x < pos.x) dir = glm::vec2(-1.0f, 0.0f);
				if (targetPos.x > pos.x) dir = glm::vec2(1.0f, 0.0f);
			}
		}
		moveVec = dir * speed * dt;
		pos += moveVec;
	}

	if (diesOob)
	{
		if (myType == 1 || myType == 5)
		if (getOutOfBounds(false, false, true)) {
			setDed(true);
		}
		if (myType == 2 || myType ==  3)
		if (getOutOfBounds(false, false, false)) {
			setDed(true);
		}
		if (myType == 4)
		if (getOutOfBounds(true, false, true)) 
		{
			setDed(true);
		}
	}

	//render
	drawMe();
}


void flyer::drawMe()
{
	glm::vec3 npos(pos.x,pos.y,0.0f);
	if (!hideSprite)
	drawSprites(myManager->getShader(0), npos, glm::vec3(r), glm::vec3(1.0f), myTex, true, animR, animC, cframe);
}

//ignoreX, ignoreY if true make the function ignore said axis
bool flyer::getOutOfBounds(bool ignoreX, bool ignoreY, bool ignoreYmax)
{

	if (!ignoreX)
	{
		if (pos.x < xmin) return true;
		if (pos.x > xmax) return true;
	}
	if (!ignoreY)
	{
		if (pos.y < ymin) return true;
		if (!ignoreYmax)
		if (pos.y > ymax) return true;
	}

	if (!ignoreYmax)
		if (pos.y > ymax) return true;

	return false;
}


void flyer::applyDamage(float amt)
{
	if (postHitTimer <= 0.0f)
	{
		hp -= amt;
		if (team==0) setPostHitTimer(1.0f);
		if (hp <= 0)
		{
			setDed(true);
			needExplode = true;
		}
	}
}