#include "../include/env.h"
#define E(x,y) ((x) + (y * NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES))

Env::Env() {
	this->R.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
	this->R.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE/2;

	this->G.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
	this->G.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE/2;

	this->B.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*3 + GRID_SIZE/2;
	this->B.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;

	this->Y.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
	this->Y.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;

	this->numPassengerStates = NUM_PASSENGER_STATES;
	this->numDestStates = NUM_DEST_STATES;
	this->numCabXStates = NUM_GRIDS_X;
	this->numCabYStates = NUM_GRIDS_Y;

	this->setupEncodeArr();
}
Env::~Env() {
}

int Env::encodeNEditState( int cabI, int cabJ, int passengerIdx, int destIdx){
	this->cab.setSpecificSpawnPosition(cabI, cabJ);
	this->passenger.setSpecificSpawnAndDropPosition(passengerIdx, destIdx);
	return this->encode(cabI, cabJ, passengerIdx, destIdx);
}

int Env::encode( int cabI, int cabJ, int passengerIdx, int destIdx){
	return cabI +\
		this->numCabXStates * cabJ +\
		this->numCabXStates * this->numCabYStates * passengerIdx +\
		this->numCabXStates * this->numCabYStates * this->numPassengerStates * destIdx;
}

void Env::setupEncodeArr(){
	for (int k = 0; k < this->numDestStates; k++)
		for (int j = 0; j < this->numPassengerStates; j++)
			for (int b = 0; b < this->numCabYStates; b++)
				for( int a = 0; a < this->numCabXStates; a++)
					encodeArr[a +\
						this->numCabXStates * b +\
						this->numCabXStates * this->numCabYStates * j +\
						this->numCabXStates * this->numCabYStates * this->numPassengerStates * k\
					] = 0;
}

void Env::initializeRewardTable(){
	for (int i = 0; i <  NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES; i ++)		
	{
		for (int j = 0; j < NUM_ACTIONS; j++){
			this->rewardTable[E(i,j)].probability = 1;
			this->rewardTable[E(i,j)].nextState = this->getNextState(i,j);
			this->rewardTable[E(i,j)].reward = this->getReward(i,j);
			this->rewardTable[E(i,j)].done = this->isDone(i,j);
		}
	}
}	

void Env::decode( int code, int& cabI, int& cabJ, int& passengerIdx, int& destIdx){
	destIdx = (int)(code/(this->numCabXStates * this->numCabYStates * this->numPassengerStates));
	code = code%(this->numCabXStates * this->numCabYStates * this->numPassengerStates);
	passengerIdx = (int)(code/(this->numCabXStates * this->numCabYStates));
	code = code%(this->numCabXStates * this->numCabYStates);
	cabJ = (int) (code/this->numCabXStates);
	cabI = code%(this->numCabXStates);
}

int Env::getNextState(int state, int action){
	int code = state;
	int cabI, cabJ, passengerIdx, destIdx;
	decode(code, cabI, cabJ, passengerIdx, destIdx);
	switch(action){
		case 0: // south
			if(checkWallCollision(cabI, cabJ, action)
			break;

		case 1: // north
			break;

		case 2: // east
			break;

		case 3: // west
			break;

		case 4: // pickup

		case 5: // dropoff
			break;
	}
	return code;	
}

int Env::getReward(int state, int action){
	int val = 0;
	switch(action){
		case 0: // south
			break;

		case 1: // north
			break;

		case 2: // east
			break;

		case 3: // west
			break;

		case 4: // pickup

		case 5: // dropoff
			break;
	}
	return val;
}

bool Env::isDone(int state, int action){
	bool flag = false;
	switch(action){
		case 0: // south

		case 1: // north

		case 2: // east

		case 3: // west

		case 4: // pickup
			break;

		case 5: // dropoff
			break;
	}
	return flag;
}
