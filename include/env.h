#include "wall.h"
#include "cab.h"
#include "passenger.h"
#include "consts.h"
#include "rewardTable.h"

class Env{
	private:
		bool encodeArr[ NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES];
		int numPassengerStates;
		int numDestStates;
		int numCabXStates;
		int numCabYStates;
		RewardTable rewardTable[ NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES * NUM_ACTIONS];

		void setupEncodeArr();
		void initializeRewardTable();

		int getNextState(int state, int action);
		int getReward(int state, int action);
		bool isDone(int state, int action);

	public:
		Wall wall;
		Cab cab;
		Passenger passenger;
		Position R,G,B,Y;
		int score;
		int currTime;
		Env();
		~Env();
		void updateScore(int amount);
		int encodeNEditState( int cabI, int cabJ, int passengerIdx, int destIdx ); // encode returns the code as well 
										// as sets the cab spawn and passenger 
										// spawn locations
		int encode( int cabI, int cabJ, int passengerIdx, int destIdx );  // only encodes the state
		void decode( int code, int& cabI, int& cabJ, int& passengerIdx, int& destIdx);
};
