#include "wall.h"
#include "cab.h"
#include "passenger.h"

class Env{
	public:
		Wall wall;
		Cab cab;
		Passenger passenger;
		Position R,G,B,Y;
		int score;
		int timeSpent;
		Env();
		~Env();
		void updateScore(int amount);
};
