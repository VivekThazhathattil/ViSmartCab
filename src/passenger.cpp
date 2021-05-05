#include "../include/passenger.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>

Passenger::Passenger() {
	this->setRandomSpawnAndDropPosition();
	for (int i = 0; i <  NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES; i++)
		this->tempPosCode[i] = 0;
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
	this->pickUpPos = getPosFromLocCode(loc1);

	this->dropOffGridPos.x = setCoords(false, loc2,'x');
	this->dropOffGridPos.y = setCoords(false, loc2,'y');
	this->dropOffPos = getPosFromLocCode(loc2);

//	printf("loc1 = %d\n",loc1);
//	printf("loc2 = %d\n",loc2);
//	printf("pickUpPos = (%d,%d)\n",this->pickUpPos.x,this->pickUpPos.y);
//	printf("dropOffPos = (%d,%d)\n",this->dropOffPos.x,this->dropOffPos.y);

	this->currPos.x = this->pickUpPos.x;
	this->currPos.y = this->pickUpPos.y;
}

Position Passenger::getPosFromLocCode(int& loc){
	Position pos;
	switch(loc){ //clockwise direction
		case 0: //R
			pos.x = 0;
			pos.y = 0;
			break;
		case 1: //G
			pos.x = 4;
			pos.y = 0;
			break;
		case 2: //B
			pos.x = 3;
			pos.y = 4;
			break;
		case 3: //Y
			pos.x = 0;
			pos.y = 4;
			break;
		default:
			pos.x = -1;
			pos.y = -1;
			break;
	}
	return pos;
}


void Passenger::setSpecificSpawnAndDropPosition(int s, int d) {
	switch(s){
		case 0:
			this->isPassengerInCab = false;
			this->pickUpCode = 'R';
			this->pickUpPos = getPosFromLocCode(s);
			this->pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			this->pickUpGridPos.y = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			break;
		case 1:
			this->isPassengerInCab = false;
			this->pickUpCode = 'G';
			this->pickUpPos = getPosFromLocCode(s);
			this->pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			this->pickUpGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE/2;
			break;
		case 2:
			this->isPassengerInCab = false;
			this->pickUpCode = 'B';
			this->pickUpPos = getPosFromLocCode(s);
			this->pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*3 + GRID_SIZE/2;
			this->pickUpGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			break;
		case 3:
			this->isPassengerInCab = false;
			this->pickUpCode = 'Y';
			this->pickUpPos = getPosFromLocCode(s);
			this->pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			this->pickUpGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			break;
	}

	switch(d){ // didn't implement check for different s and d, assuming the user invokes correctly
		case 0:
			this->dropOffCode = 'R';
			this->dropOffPos = getPosFromLocCode(d);
			this->dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			this->dropOffGridPos.y = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE/2;
			break;
		case 1:
			this->dropOffCode = 'G';
			this->dropOffPos = getPosFromLocCode(d);
			this->dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			this->dropOffGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE/2;
			break;
		case 2:
			this->dropOffCode = 'B';
			this->dropOffPos = getPosFromLocCode(d);
			this->dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2 + GRID_SIZE*3 + GRID_SIZE/2;
			this->dropOffGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2 + GRID_SIZE*4 + GRID_SIZE/2;
			break;
		case 3:
			this->dropOffCode = 'Y';
			this->dropOffPos = getPosFromLocCode(d);
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
