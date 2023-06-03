#pragma once

#include "ourGraphics.h"
#include "ourGraphicsFreeType.h"

class flyer
{
private:
	glm::vec2 pos;        //object coords
	glm::vec2 targetPos;  //target coord
	glm::vec2 dir;        //object direction
	float speed;          //object speed (scalar)
	float r;              //object radius
	float collisionR;     //collision radius
	int myType;           //object type
	unsigned int myTex;   //texture
	int animR, animC;     //animation rows, columns
	int team;             //0 - user; 1 - enemy
	bool isBullet;        //treat this entity as a bullet
	float reloadTime;     //time to be able to shoot again
	float reloadTimer;    //handles when shot can be fired
	int bulletId;         //bullet actor id
	bool canShoot;        //actor is ready to fire a bullet
	bool usrShootCall;    //user's shoot input is pressed
	int scoreValue;		  //how much points is given for shooting down	
	int myTrajectory;     //0 - straight, 1 - sin

	float trajA, trajB;   //trajectory parameters if needed

	float hp;             //current hitpoints
	float dmg;            //hp damage dealt by if bullet

	bool isPowerUp;		  //flyer is a powerup	

	bool isBoss;          //flyer is boss

	int powerUpType;      //1 - healing
	
	//working values
	int cframe;
	float myTimer;
	float fps;
	int nframes;

	bool diesOob;
	float xmin, xmax, ymin, ymax;

	bool ded;
	bool needExplode;

	bool hideSprite; //used for iframe blink

	float inCounter;

	float postHitTimer; //provides iframes for player

	OGLManager *myManager;
	glm::vec2 *dirAxis;
	glm::vec2 plrPos;

public:
	flyer(int vmyType, glm::vec2 vpos, glm::vec2 tpos,
		OGLManager *vmyManager, float vr,
		unsigned int vmyTex, int vanimR, int vanimC);
	void processInternals(float dt, glm::vec2 dirAxR);
	void drawMe();
	void applyDamage(float amt);
	void increaseHp() { hp++; };
	void increaseHp(int amt) { hp+=amt; };
	void setPlrPos(glm::vec2 bPPos) { plrPos = bPPos; };
	void setTargetPos(glm::vec2 bPPos) { targetPos = bPPos; };
	void setCanShoot(bool vCanShoot) { canShoot = vCanShoot; };
	void setDed(bool v) { ded = v; };
	void setUserShootCall(bool v) { usrShootCall = v; };
	void setIsBullet(bool v) { isBullet = v; };
	void setCollisionRadius(float v) { collisionR = v; };
	void setTeam(int v) { team = v; };
	void setHp(float v) { hp = v; };
	void setPostHitTimer(float v) { postHitTimer = v; };

	bool getOutOfBounds(bool restrictX, bool restrictY, bool ignoreYmax);
	int getBulletId() { return bulletId; };
	glm::vec2 getPos() { return pos; };
	glm::vec2 getTargetPos() { return targetPos; };
	bool getCanShoot() { return canShoot; };
	bool getDed() { return ded; }	
	bool getIsBullet() { return isBullet; };
	bool getIsPowerUp() { return isPowerUp; };
	int getPowerupType() { return powerUpType; }
	float getRadius() { return r; };
	float getCollisionRadius() { return r; };
	int getTeam() { return team; };
	bool getNeedExplode() { return needExplode; };
	float getDamage() { return dmg; };
	float getHp() { return hp; }
	int getScoreValue() { return scoreValue; };
	int getMyType() { return myType; };
	bool getIsBoss() { return isBoss; };
};



