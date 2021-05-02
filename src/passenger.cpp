#include "../include/passenger.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>

Passenger::Passenger() {
	this->setRandomSpawnPosition();
}
Passenger::~Passenger(){}

void Passenger::setRandomSpawnPosition(){
	std::srand(time(0));
	int loc1 = rand()%4;
	int loc2 = rand()%4;
	while(loc2==loc1)
		loc2 = rand()%4;
	this->pickUpPos.x = setCoords(true, loc1,'x');
	this->pickUpPos.y = setCoords(true, loc1,'y');
	this->dropOffPos.x = setCoords(false, loc2,'x');
	this->dropOffPos.y = setCoords(false, loc2,'y');
}

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
