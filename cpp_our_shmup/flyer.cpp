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
	team = 1;

	//set up ship/bullet specific properties
	if (myType == 0) //plr ship
	{
		speed = 300.0f;
		diesOob = false;
		reloadTime = 0.2f;
		reloadTimer = reloadTime;
		bulletId = 2;
		hp = 3.0f;
		team = 0;
	}

	if (myType == 1) //en 1
	{
		speed = 400.0f;
		reloadTime = 1.2f;
		reloadTimer = reloadTime;
		bulletId = 3;
		hp = 1.0f;
		scoreValue = 100;
	}

	if (myType == 2) //plr bullet
	{
		speed = 600.0f;
		reloadTime = 0.0f;
		reloadTimer = reloadTime;
		bulletId = -1;
		dir = glm::vec2(0.0f,1.0f);
		r = 8.0f;
		dmg = 1.0f;
		isBullet = true;
		team = 0;
	}

	if (myType == 3) //en bullet
	{
		speed = 500.0f;
		reloadTime = 0.0f;
		reloadTimer = reloadTime;
		bulletId = -1;
		dir = glm::normalize(tpos-pos);
		r = 8.0f;
		dmg = 1.0f;
		isBullet = true;
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

	if (myType == 1) //enemy 1
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

	if (diesOob)
	{
		if (myType == 1)
		if (getOutOfBounds(false, false, true)) {
			setDed(true);
		}
		if (myType == 2 || myType ==  3)
		if (getOutOfBounds(false, false, false)) {
			setDed(true);
		}
	}

	//render
	drawMe();
}


void flyer::drawMe()
{
	glm::vec3 npos(pos.x,pos.y,0.0f);
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
	hp -= amt;
	if (hp <= 0)
	{
		setDed(true);
		needExplode = true;
	}
}