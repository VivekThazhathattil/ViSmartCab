#include "wall.h"
#include "cab.h"
#include "passenger.h"

class Env{
	private:
		Wall wall;
		Cab cab;
		Passenger passenger;
		int score;
		int timeSpent;
	public:	
		Env();
		~Env();
		void updateScore(int amount);
};
