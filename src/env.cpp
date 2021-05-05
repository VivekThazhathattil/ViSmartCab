#include "../include/env.h"
#include <time.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cmath>
#include "../include/position.h"

//#include <cstdlib>
//#include <cstdio>
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

	if(std::filesystem::exists("./saveData/qTable.dat")){
		this->getQTableFromFile();		
	}
	else{
		this->resetQTable();
		this->iterator = 0;
	}
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
			this->passenger.setPassengerStatus(false);// getNextState may cause passenger status to vary
			this->rewardTable[E(i,j)].reward = this->getReward(i,j);
			this->rewardTable[E(i,j)].done = this->isDone(i,j);
		}
	}	
	saveRewardTableToFile("./saveData/rewardTable.dat");
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
	Position passengerSpawnPos, passengerDestPos;
	decode(code, cabI, cabJ, passengerIdx, destIdx);
	if (passengerIdx != 4)
		passengerSpawnPos = this->passenger.getPosFromLocCode(passengerIdx);
	passengerDestPos = this->passenger.getPosFromLocCode(destIdx);

	switch(action){
		case 0: // south
			if(!this->wall.checkWallCollision(cabI, cabJ, action))
				code = this->encode(cabI, cabJ+1, passengerIdx, destIdx);	
			break;

		case 1: // north
			if(!this->wall.checkWallCollision(cabI, cabJ, action))
				code = this->encode(cabI, cabJ-1, passengerIdx, destIdx);	
			break;

		case 2: // east
			if(!this->wall.checkWallCollision(cabI, cabJ, action))
				code = this->encode(cabI+1, cabJ, passengerIdx, destIdx);	
			break;

		case 3: // west
			if(!this->wall.checkWallCollision(cabI, cabJ, action))
				code = this->encode(cabI-1, cabJ, passengerIdx, destIdx);	
			break;

		case 4: // pickup
			if (passengerIdx != 4)
				if ( passengerSpawnPos.x == cabI && passengerSpawnPos.y == cabJ && passengerIdx != 4){
					this->passenger.tempPosCode = passengerIdx;
					passengerIdx = 4;
					this->passenger.setPassengerStatus(true);
					code = this->encode(cabI, cabJ, passengerIdx, destIdx);	
				}
			break;
		case 5: // dropoff
			if (passengerIdx == 4 &&\
					passengerDestPos.x == cabI &&\
					passengerDestPos.y == cabJ\
			   ){
//				printf("passenger dropped off at (%d,%d)\n",cabI,cabJ);
				this->passenger.setPassengerStatus(false);
				code = this->encode(cabI, cabJ, this->passenger.tempPosCode, destIdx);	

			}
			break;
	}
	return code;	
}

int Env::getReward(int state, int action){
	int cabI, cabJ, passengerIdx, destIdx;
	decode(state, cabI, cabJ, passengerIdx, destIdx);
	Position passengerSpawnPos, passengerDestPos;
	if (passengerIdx != 4)
		passengerSpawnPos = this->passenger.getPosFromLocCode(passengerIdx);
	passengerDestPos = this->passenger.getPosFromLocCode(destIdx);
	int reward = 0;
	switch(action){
		case 0: // south

		case 1: // north

		case 2: // east

		case 3: // west
			if(this->wall.checkWallCollision(cabI, cabJ, action))
				reward = -2;
			else
				reward = -1; // all movements attribute to -1 reward
			break;

		case 4: // pickup
			/* correct pickup deserves positive reinforcement */
			if (passengerIdx != 4){
				if (	passengerSpawnPos.x == cabI &&\
				       	passengerSpawnPos.y == cabJ\
				)
					reward = 10;
				else
					reward = -5;					
			}
			else
				reward = -5; /* wrong pickup needs to be punished */
			break;

		case 5: // dropoff
			if(\
			passengerDestPos.x == cabI &&\
			passengerDestPos.y == cabJ &&\
			passengerIdx == 4\
			)
				reward = 50;
			else
				reward = -5; /* wrong drop off attempt punished */
			break;
	}
	return reward;
}

bool Env::isDone(int state, int action){
	int cabI, cabJ, passengerIdx, destIdx;
	decode(state, cabI, cabJ, passengerIdx, destIdx);
	bool flag = false;
	Position passengerDestPos= this->passenger.getPosFromLocCode(destIdx);
	switch(action){
		case 0: // south

		case 1: // north

		case 2: // east

		case 3: // west

		case 4: // pickup
			break;

		case 5: // dropoff
			if(\
			passengerDestPos.x == cabI &&\
			passengerDestPos.y == cabJ &&\
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
	for(int j = 0; j < NUM_ACTIONS; j++){
		if(this->qTable[E(state,j)] >= maxVal){
			maxVal = this->qTable[E(state,j)];
			jCandidate = j;
		}
	}
	return jCandidate;
}

void Env::step(int actionCode, int state, int& nextState, int& reward, bool& done){
	int code = state;
	int cabI, cabJ, passengerIdx, destIdx;
	decode(code, cabI, cabJ, passengerIdx, destIdx);

/*	nextState = this->getNextState(state, actionCode);
	if (!this->passenger.getPassengerStatus() && passengerIdx == 4)
		this->passenger.setPassengerStatus(true);
	reward = this->getReward(state, actionCode);
	done = this->isDone(state, actionCode); */

	nextState = this->rewardTable[E(state,actionCode)].nextState;
	if (!this->passenger.getPassengerStatus() && passengerIdx == 4)
		this->passenger.setPassengerStatus(true);
	reward = this->rewardTable[E(state,actionCode)].reward;
	done = this->rewardTable[E(state,actionCode)].done;
}

double Env::getMaxQForState(int& state){
	float maxVal = - 10000.0;
	for(int j = 0; j < NUM_ACTIONS; j++){
		if(this->qTable[E(state,j)] > maxVal){
			maxVal = this->qTable[E(state,j)];
		}
	}
	return maxVal;
}

void Env::reset(){
/* reset cab position, passenger position */
	this->cab.setRandomSpawnPosition();
	this->passenger.setRandomSpawnAndDropPosition();
}

std::string Env::actionCodeToString(int& code){
	std::string text;
	switch(code){
		case 0:
			text = "south";
			break;
		case 1:
			text = "north";
			break;
		case 2:
			text = "east";
			break;
		case 3:
			text = "west";
			break;
		case 4:
			text = "pickup";
			break;
		case 5:
			text = "dropoff";
			break;
	}
	return text;
}

void Env::saveQTableToFile(std::string fileName){
	std::ofstream qFile;
	qFile.open(fileName, std::fstream::trunc);
	if(!qFile.is_open()){
	        std::cerr << "There was a problem opening the input file!\n";
		exit(1);
	}
	for (int i = 0; i <= NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES * NUM_ACTIONS; i++){
		if(i==0)
			qFile << this->iterator << std::endl;	
		else
		{
			if(!std::isinf(qTable[i-1]))
				qFile << this->qTable[i-1] << std::endl;
			else
				qFile << NEG_LIM << std::endl;
		}
	}
	qFile.close();
}

void Env::saveRewardTableToFile(std::string fileName){
	int cabI0, cabI1, cabJ0, cabJ1, psg_idx0, dest_idx0, psg_idx1, dest_idx1;
	std::ofstream qFile;
	qFile.open(fileName, std::fstream::trunc);
	if(!qFile.is_open()){
	        std::cerr << "There was a problem opening the input file!\n";
		exit(1);
	}
	qFile << "(state,action)   init_cab_pos    next_cab_pos    psg_spawn_idx0    psg_spawn_idx1    psg_dest_idx0    psg_dest_idx1    reward\n";
	for (int i = 0; i < NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES; i++)
	for (int j = 0; j < NUM_ACTIONS; j++){
		this->decode((i)%(NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES),cabI0,cabJ0,psg_idx0,dest_idx0);
		this->decode(rewardTable[E(i,j)].nextState,cabI1,cabJ1,psg_idx1,dest_idx1);
		qFile << "(" << i << "," << actionCodeToString(j) << ")" << "\t (" << cabI0 << "," <<cabJ0 << ") ---> (" << cabI1 << "," << cabJ1 << ")\t " << psg_idx0 << " ---> " << psg_idx1 << "\t" << dest_idx0 << " ---> " << dest_idx1 << " \t " << rewardTable[E(i,j)].reward << std::endl;
	}
	qFile.close();
}

void Env::getQTableFromFile(){
	std::ifstream qFile;
	qFile.open("./saveData/qTable.dat", std::fstream::in);
	if (!qFile.is_open()) {
	        std::cerr << "There was a problem opening the input file!\n";
	        exit(1);
    	}
	double num = 0.0;
	int i = 0;
	while(qFile >> num && i <= NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES * NUM_ACTIONS)
	{
		if (i == 0)
			this->iterator = num;
		else{
			if(!std::isinf(num))
				this->qTable[i-1] = num;
			else
				this->qTable[i-1] = NEG_LIM;
		}
		i++;
	}
}
