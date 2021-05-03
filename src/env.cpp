#include "../include/env.h"
#include <time.h>
#include <cstdlib>
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
	this->initializeRewardTable();
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

void Env::resetQTable(){
	for (int i = 0; i<  NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES * NUM_ACTIONS ; i++)
		this->qTable[i] = 0;
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
			if(this->wall.checkWallCollision(cabI, cabJ, action))
				code = state;
			else
				code = this->encode(cabI, cabJ+1, passengerIdx, destIdx);	
			break;

		case 1: // north
			if(this->wall.checkWallCollision(cabI, cabJ, action))
				code = state;
			else
				code = this->encode(cabI, cabJ-1, passengerIdx, destIdx);	
			break;

		case 2: // east
			if(this->wall.checkWallCollision(cabI, cabJ, action))
				code = state;
			else
				code = this->encode(cabI+1, cabJ, passengerIdx, destIdx);	
			break;

		case 3: // west
			if(this->wall.checkWallCollision(cabI, cabJ, action))
				code = state;
			else
				code = this->encode(cabI-1, cabJ, passengerIdx, destIdx);	
			break;

		case 4: // pickup
			if (passengerIdx != 4 &&\
					this->passenger.getPos(0,0) == cabI &&\
					this->passenger.getPos(0,1) == cabJ\
			   ){
				passengerIdx = 4;
				code = this->encode(cabI, cabJ, passengerIdx, destIdx);	
			}
			break;
		case 5: // dropoff
			if (passengerIdx == 4 &&\
					this->passenger.getPos(1,0) == cabI &&\
					this->passenger.getPos(1,1) == cabJ\
			   ){
				char a = this->passenger.getCode(0);
				if (a == 'R') passengerIdx = 0;
				else if (a == 'G') passengerIdx = 1;
				else if (a == 'B') passengerIdx = 2;
				else if (a == 'Y') passengerIdx = 3;
				code = this->encode(cabI, cabJ, passengerIdx, destIdx);	

			}
			break;
	}
	return code;	
}

int Env::getReward(int state, int action){
	int cabI, cabJ, passengerIdx, destIdx;
	decode(state, cabI, cabJ, passengerIdx, destIdx);
	int reward = 0;
	switch(action){
		case 0: // south

		case 1: // north

		case 2: // east

		case 3: // west
			reward = -1; // all movements attribute to -1 reward
			break;

		case 4: // pickup
			reward = -10;
			break;

		case 5: // dropoff
			if(\
			this->passenger.getPos(1,0) == cabI &&\
			this->passenger.getPos(1,1) == cabJ &&\
			passengerIdx == 4\
			)
				reward = 20;
			else
				reward = -10;
			break;
	}
	return reward;
}

bool Env::isDone(int state, int action){
	int cabI, cabJ, passengerIdx, destIdx;
	decode(state, cabI, cabJ, passengerIdx, destIdx);
	bool flag = false;
	switch(action){
		case 0: // south

		case 1: // north

		case 2: // east

		case 3: // west

		case 4: // pickup
			break;

		case 5: // dropoff
			if(\
			this->passenger.getPos(1,0) == cabI &&\
			this->passenger.getPos(1,1) == cabJ &&\
			passengerIdx == 4\
			)
				flag = true;
			break;
	}
	return flag;
}

void Env::learn(){
	std::srand(time(0));
	int epochs, penalties, reward;
	bool done;
	int cabI, cabJ, passengerIdx, destIdx, state, nextState;
	float oldQ, newQ, nextMaxQ;
	int actionCode;
	for (int i = 0; i < NUM_ITERATIONS; i++){
		this->reset(); //reset our sample space
		cabI = this->cab.getSpawnPosition('x');
		cabJ = this->cab.getSpawnPosition('y');
		if (this->passenger.getPassengerStatus())
			passengerIdx = 4;
		else{
			if(this->passenger.getCode(0) == 'R')
				passengerIdx = 0;
			else if(this->passenger.getCode(0) == 'G')
				passengerIdx = 1;
			else if(this->passenger.getCode(0) == 'B')
				passengerIdx = 2;
			else if(this->passenger.getCode(0) == 'Y')
				passengerIdx = 3;
		}	
		if(this->passenger.getCode(1) == 'R')
			destIdx = 0;
		else if(this->passenger.getCode(1) == 'G')
			destIdx = 1;
		else if(this->passenger.getCode(1) == 'B')
			destIdx = 2;
		else if(this->passenger.getCode(1) == 'Y')
			destIdx = 3;
		state = this->encode(cabI, cabJ, passengerIdx, destIdx);
		epochs = 0;
		penalties = 0;
		reward = 0;
		done = false;
		while(!done){
			float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
			if(r < EPSILON)
				actionCode = std::rand()%NUM_ACTIONS; //exploration
			else
				actionCode = this->getActionForMaxQValue(state); //exploitation
			this->step(actionCode, state, nextState, reward, done);
			oldQ = this->qTable[E(state,actionCode)];
			nextMaxQ = this->getMaxQForState(state);
			newQ = ((1-ALPHA) * oldQ)+\
			       (ALPHA * (reward + GAMMA * nextMaxQ));
			this->qTable[E(state, actionCode)] = newQ;
			if (reward == -10)
				penalties += 1;
			state = nextState;
			epochs += 1;
		}

	}
}

int Env::getActionForMaxQValue(int& state){
/* Looks in the QTable for a particular state and find the action with the max q-value*/
	float maxVal = - 10000.0;
	int jCandidate = 0;
	int j;
	for(j = 0; j < NUM_ACTIONS; j++){
		if(this->qTable[E(state,j)] > maxVal){
			maxVal = this->qTable[E(state,j)];
			jCandidate = j;
		}
	}
	return j;
}

void Env::step(int actionCode, int state, int& nextState, int& reward, bool& done){
	int code = state;
	int cabI, cabJ, passengerIdx, destIdx;
	decode(code, cabI, cabJ, passengerIdx, destIdx);

	nextState = this->getNextState(state, actionCode);
	if (!this->passenger.getPassengerStatus() && passengerIdx == 4)
		this->passenger.setPassengerStatus(true);
	reward = this->getReward(state, actionCode);
	done = this->isDone(state, actionCode);
}

float Env::getMaxQForState(int& state){
	float maxVal = - 10000.0;
	int jCandidate = 0;
	for(int j = 0; j < NUM_ACTIONS; j++){
		if(this->qTable[E(state,j)] > maxVal){
			maxVal = this->qTable[E(state,j)];
			jCandidate = j;
		}
	}
	return maxVal;
}

void Env::reset(){
/* reset cab position, passenger position */
	this->cab.setRandomSpawnPosition();
	this->passenger.setRandomSpawnAndDropPosition();
}
