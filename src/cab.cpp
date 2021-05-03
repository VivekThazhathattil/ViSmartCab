#include "../include/cab.h"
#include <time.h>
#include <cstdlib>

Cab::Cab() {
	this->setRandomSpawnPosition();
}
Cab::~Cab() {}

int Cab::getSpawnPosition(char i){
	if (i == 'x') return this->spawnPos.x;
	else return this->spawnPos.y;
}

int Cab::getCurrPosition(char i){
	if (i == 'x') return this->currPos.x;
	else return this->currPos.y;
}

void Cab::setCurrPosition(int& x, int& y){
	this->currPos.x = x;
	this->currPos.y = y;
}

void Cab::setRandomSpawnPosition(){
	std::srand(time(0));
	this->spawnPos.x = std::rand()%5;
	this->spawnPos.y = std::rand()%5;
	this->currPos.x = this->spawnPos.x;
	this->currPos.y = this->spawnPos.y;
}

void Cab::setSpecificSpawnPosition(int x, int y){
	this->spawnPos.x = x;
	this->spawnPos.y = y;
	this->currPos.x = this->spawnPos.x;
	this->currPos.y = this->spawnPos.y;
}
