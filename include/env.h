#include "wall.h"
#include "cab.h"
#include "passenger.h"
#include "consts.h"

class Env{
	private:
		bool encodeArr[ NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES];
		int numPassengerStates;
		int numDestStates;
		int numCabXStates;
		int numCabYStates;

		void setupEncodeArr();

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
		int encode( int cabI, int cabJ, int passengerIdx, int destIdx ); // encode returns the code as well 
										// as sets the cab spawn and passenger 
										// spawn locations
};
