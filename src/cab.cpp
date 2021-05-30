#include "../include/cab.h"
#include <cstdlib>
#include <time.h>

Cab::Cab() { setRandomSpawnPosition(); }
Cab::~Cab() {}

int Cab::getSpawnPosition(char i) {
  if (i == 'x')
    return spawnPos.x;
  else
    return spawnPos.y;
}

int Cab::getCurrPosition(char i) {
  if (i == 'x')
    return currPos.x;
  else
    return currPos.y;
}

void Cab::setCurrPosition(int &x, int &y) {
  currPos.x = x;
  currPos.y = y;
}

void Cab::setRandomSpawnPosition() {
  spawnPos.x = std::rand() % 5;
  spawnPos.y = std::rand() % 5;
  currPos.x = spawnPos.x;
  currPos.y = spawnPos.y;
}

void Cab::setSpecificSpawnPosition(int x, int y) {
  spawnPos.x = x;
  spawnPos.y = y;
  currPos.x = spawnPos.x;
  currPos.y = spawnPos.y;
}
