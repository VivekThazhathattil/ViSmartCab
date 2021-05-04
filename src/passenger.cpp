#include "../include/passenger.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>

Passenger::Passenger() {
	this->setRandomSpawnAndDropPosition();
}
Passenger::~Passenger(){}

void Passenger::setRandomSpawnAndDropPosition(){
	this->isPassengerInCab = false;
	std::srand(time(0));
	int loc1 = rand()%4;
	int loc2 = rand()%4;
	while(loc2==loc1)
		loc2 = rand()%4;
	this->pickUpGridPos.x = setCoords(true, loc1,'x');
	this->pickUpGridPos.y = setCoords(true, loc1,'y');
	this->pickUpPos.x = getPosFromLocCode(loc1,'x');
	this->pickUpPos.y = getPosFromLocCode(loc1,'y');

	this->dropOffGridPos.x = setCoords(false, loc2,'x');
	this->dropOffGridPos.y = setCoords(false, loc2,'y');
	this->dropOffPos.x = getPosFromLocCode(loc2, 'x');
	this->dropOffPos.y = getPosFromLocCode(loc2, 'y');

	this->currPos.x = this->pickUpPos.x;
	this->currPos.y = this->pickUpPos.y;
}

int Passenger::getPosFromLocCode(int& loc, char a){
	int pos;
	switch(loc){
		case 0: //R
			if ( a == 'x') pos = 0;
			else pos = 0;
		case 1: //G
			if ( a == 'x') pos = 4;
			else pos = 0;
		case 2: //B
			if ( a == 'x') pos = 3;
			else pos = 4;
		case 3: //Y
			if ( a == 'x') pos = 0;
			else pos = 4;

	}
	return pos;
}


void Passenger::setSpecificSpawnAndDropPosition(int s, int d) {
	switch(s){
		case 0:
			this->isPassengerInCab = false;
			this->pickUpCode = 'R';
			this->pickUpPos.x = getPosFromLocCode(s,'x');
			this->pickUpPos.y = getPosFromLocCode(s,'y');
			this->pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			this->pickUpGridPos.y = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			break;
		case 1:
			this->isPassengerInCab = false;
			this->pickUpCode = 'G';
			this->pickUpPos.x = getPosFromLocCode(s,'x');
			this->pickUpPos.y = getPosFromLocCode(s,'y');
			this->pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			this->pickUpGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE/2;
			break;
		case 2:
			this->isPassengerInCab = false;
			this->pickUpCode = 'B';
			this->pickUpPos.x = getPosFromLocCode(s,'x');
			this->pickUpPos.y = getPosFromLocCode(s,'y');
			this->pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*3 + GRID_SIZE/2;
			this->pickUpGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			break;
		case 3:
			this->isPassengerInCab = false;
			this->pickUpCode = 'Y';
			this->pickUpPos.x = getPosFromLocCode(s,'x');
			this->pickUpPos.y = getPosFromLocCode(s,'y');
			this->pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			this->pickUpGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			break;
	}

	switch(d){ // didn't implement check for different s and d, assuming the user invokes correctly
		case 0:
			this->dropOffCode = 'R';
			this->dropOffPos.x = getPosFromLocCode(d, 'x');
			this->dropOffPos.y = getPosFromLocCode(d, 'y');
			this->dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			this->dropOffGridPos.y = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			break;
		case 1:
			this->dropOffCode = 'G';
			this->dropOffPos.x = getPosFromLocCode(d, 'x');
			this->dropOffPos.y = getPosFromLocCode(d, 'y');
			this->dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			this->dropOffGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE/2;
			break;
		case 2:
			this->dropOffCode = 'B';
			this->dropOffPos.x = getPosFromLocCode(d, 'x');
			this->dropOffPos.y = getPosFromLocCode(d, 'y');
			this->dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*3 + GRID_SIZE/2;
			this->dropOffGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			break;
		case 3:
			this->dropOffCode = 'Y';
			this->dropOffPos.x = getPosFromLocCode(d, 'x');
			this->dropOffPos.y = getPosFromLocCode(d, 'y');
			this->dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			this->dropOffGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			break;
	}

	this->currPos.x = this->pickUpPos.x;
	this->currPos.y = this->pickUpPos.y;

} //Problem: source (s) can be inside the taxi, which is not a valid spawn location
// In that case, retain the previous spawn location. s= 0,1,2,3,4 corresponds to R,G,B,Y,insideCab

int Passenger::setCoords(bool isPickup, int key, char coord){
	if (key == 0){
		if (isPickup)
			this->pickUpCode = 'R';
		if (!isPickup)
			this->dropOffCode = 'R';
		if (coord == 'x'){
			return (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
		}
		else if (coord == 'y')
			return (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE/2;
	}
	else if (key == 1){
		if (isPickup)
			this->pickUpCode = 'G';
		if (!isPickup)
			this->dropOffCode = 'G';
		if (coord == 'x')
			return (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
		else if (coord == 'y')
			return (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE/2;
	}
	else if (key == 2){
		if (isPickup)
			this->pickUpCode = 'B';
		if (!isPickup)
			this->dropOffCode = 'B';
		if (coord == 'x')
			return (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*3 + GRID_SIZE/2;
		else if (coord == 'y')
			return (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
	}
	else if (key == 3){
		if (isPickup)
			this->pickUpCode = 'Y';
		if (!isPickup)
			this->dropOffCode = 'Y';
		if (coord == 'x')
			return (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
		else if (coord == 'y')
			return (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
	}
	else{
		printf("Error in key generation (not between 0 to 4)!\n");
	}
	return -1;
}

char Passenger::getCode(int flag){
	if (flag == 0){
		return this->pickUpCode;
	}
	return this->dropOffCode;
}

int Passenger::getPos(int i,int j){
	int pos = 0;
	if (i == 0){
		if (j == 0)
			pos = this->pickUpPos.x;
		else
			pos = this->pickUpPos.y;
	}
	else{
		if (j == 0)
			pos = this->dropOffPos.x;
		else
			pos = this->dropOffPos.y;
	}
	return pos;
}

bool Passenger::getPassengerStatus(){
	return this->isPassengerInCab;
}

void Passenger::setPassengerStatus(bool val){
	this->isPassengerInCab = val;
}
