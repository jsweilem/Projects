// Fundamentals of Computing 20311-Joseph Sweilem
// This program is the interface for the Fighter class

class Fighter
{
	public:
		// Constructors and Destructors
		Fighter();
		Fighter(float, float);
		~Fighter();
		
		// Access Methods
		float getX();
		float getY();
		int getCX();
		int getCY();
		int getCenterX();
		int getCenterY(); 
		float getLives();
		bool getIsFired();

		// Utility Methods
		void changeTheta(float);
		void resetHit();
		void fireGun();
		void moveCircle();
		void decreaseLife();
		void speedFighter();
		void drawFighter(int, int);
	private:
		float x, y;
		float theta, ct;
		int circleX, circleY;
		bool isFired, isHit;
		float lives;
		int centerX, centerY;
}
;
