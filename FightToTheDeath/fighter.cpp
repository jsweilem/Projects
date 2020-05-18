// Fundamentals of Computing 20311-Joseph Sweilem
// This program is the implementation for the Fighter class

#include <iostream>
using namespace std;

#include <cstdlib>
#include <cmath>
#include <time.h>
#include <unistd.h>

#include "gfx.h"
#include "fighter.h"

// Default Constructor
Fighter::Fighter() {
	x=600;
	y=400;
	theta=0;
	circleX=x;
	circleY=y;
	ct=theta;
	isFired=false;
	isHit=false;
	lives=5;
	drawFighter(200+x, 400);
}

// Regular Constructor
Fighter::Fighter(float xSpot, float ySpot) {
	x=xSpot;
	y=ySpot;
	theta=0;
	circleX=x;
	circleY=y;
	ct=theta;
	isFired=false;
	isHit=false;
	lives=5;
	drawFighter(200+x, 400);
}

// Destructor
Fighter::~Fighter() {}

// Access Methods
float Fighter::getX() {return x;}
float Fighter::getY() {return y;}
int Fighter::getCX() {return circleX;}
int Fighter::getCY() {return circleY;}
int Fighter::getCenterX() {return centerX;}
int Fighter::getCenterY() {return centerY;}
float Fighter::getLives() {return lives;}
bool Fighter::getIsFired() {return isFired;}

// Utility Methods

// Changes the angle at which the fighter moves
void Fighter::changeTheta(float ct) {theta+=ct;}

// Resets hit and circle
void Fighter::resetHit() {isHit=false;}

// Fires a circle at the opponent
void Fighter::fireGun() {
	circleX=x;
	circleY=y;
	ct=theta;
	circleX++;
	circleY++;
	isFired=true;
}

// The animation of a shot moving toward the opponent
void Fighter::moveCircle() {
	circleX+=10*cos(ct);
	circleY+=10*sin(ct);
	gfx_circle(circleX, circleY, 2);
}

// Removes a life from the counter
void Fighter::decreaseLife() {
	if(!isHit) lives--;
	isHit=true;
}

// Based on user input, changes the position of the boss if it still has lives and speeds the fighter up when it has fewer lives
void Fighter::speedFighter() {
	if(lives==0) return;
	x+=(5/lives)*cos(theta);
	y+=(5/lives)*sin(theta);
}

// Creates the visual movement of the ship calculated in speedFighter
void Fighter::drawFighter(int xc, int yc) {
	speedFighter();
	if(x<30) x=30;
	if(x>xc-30) x=xc-30;
	if(y<80) y=80;
	if(y>yc-30) y=yc-30;

	float tipX=x+55*cos(theta);
	float tipY=y+55*sin(theta);
	float midX=x+19*cos(theta);
	float midY=y+19*sin(theta);
	float upX=x+8*cos(theta+1.53);
	float upY=y+8*sin(theta+1.53);
	float downX=x-8*sin(theta+1.53);
	float downY=y-8*sin(theta+1.53);

	centerX=midX;
	centerY=midY;

	gfx_line(tipX, tipY, upX, upY);
	gfx_line(tipX, tipY, downX, downY);
	gfx_line(midX, midY, upX, upY);
	gfx_line(midX, midY, downX, downY);
	int centerX=.5*(x+tipX);
	int centerY=.5*(y+tipY);
	gfx_circle(midX, midY, 10);
}













