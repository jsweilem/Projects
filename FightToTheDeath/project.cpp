// Fundamentals of Computing 20311-Joseph Sweilem
// This program acts as the driver for Fighter class and runs the game

#include <iostream>
using namespace std;

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <time.h>
#include <unistd.h>

#include "gfx.h"
#include "fighter.h"

int main()
{
	int w=1200, h=650;
	char input;
	bool flag=true;

	gfx_open(w, h, "Battle to the Death!");

	// Creates the fighters and places them on the null board
	Fighter fighter(w/4, h/2);
	Fighter boss(3*w/4, h/2);

	// Displays the instructions
	gfx_text(w/2-50, h-50, "HIT SPACE TO BEGIN THE BATTLE!");
	gfx_text(w/3, 75, "INSTRUCTIONS");
	gfx_text(w/3, 100,"Fighter uses 'a' and 'd' to turn and 's' to shoot");
	gfx_text(w/3, 125, "Boss uses 'j' and 'l' to turn");
	gfx_text(w/3, 150, "The boss has 5 lives. Good luck!");
	gfx_text(10, 20, "Boss Lives: 5");

	// Waits for the spacebar or 'q' to begin the game or quit
	while(input!=32) {
		input=gfx_wait();
		
		if(input=='q') {
			return 1;
		}
	}
	gfx_clear();
	
	fighter.drawFighter(w, h);
	boss.drawFighter(w, h);

	// Game loop
	while(flag) {
		if(gfx_event_waiting()) {
			input=gfx_wait();

			// The following inputs either change the direction of the fighter or shoot at the opponent
			if(input=='q') {
				break;
			}
			else if(input=='a') {
				fighter.changeTheta(-0.30);
			}
			else if(input=='d') {
				fighter.changeTheta(0.30);
			}
			else if(input=='s') {
				fighter.fireGun();
				boss.resetHit();
			}
			else if(input=='j') {
				boss.changeTheta(-0.47);
			}
			else if(input=='l') {
				boss.changeTheta(0.47);
			}
		}
	

		gfx_clear();
		
		// Displays the lives in real time 
		gfx_text(10, 40, "Boss Lives: ");
		gfx_text(105, 40, (to_string((int)boss.getLives()).c_str()));

		// End cases when one of the ships loses all of their lives
		if(boss.getLives()==0) {
			gfx_text(w/2, h/2, "CONGRATS! YOU DESTROYED THE BOSS!!!");
			break;
		}

		// Create new fighter with new location after input
		fighter.drawFighter(w, h);
		boss.drawFighter(w, h);
		
		// If the shoot button is recorded, then a circle is moved across the screen	
		if(fighter.getIsFired()) {
			fighter.moveCircle();
		}

		
		// If a collision of the fired circle and the center circle of the boss occurs, that boss loses a life
		if(boss.getCenterX()+15>fighter.getCX() && boss.getCenterX()-15<fighter.getCX()) {
			if(boss.getCenterY()-15<fighter.getCY() && boss.getCenterY()+15>fighter.getCY()) {
				boss.decreaseLife();
			}
		}
		usleep(10000);
	}
	
	// Once game is finished, waits for the user to exit game
	while(input!='q') {
		gfx_text(w/2, h/2+20, "Press 'q' to exit");
		input=gfx_wait();
	}

	return 0;
}












