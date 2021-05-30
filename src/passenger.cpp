#include "../include/passenger.h"
#include <cstdio>
#include <cstdlib>
#include <time.h>

Passenger::Passenger() {
  setRandomSpawnAndDropPosition();
  for (int i = 0;
       i < NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES;
       i++)
    tempPosCode[i] = 0;
}
Passenger::~Passenger() {}

void Passenger::setRandomSpawnAndDropPosition() {
  isPassengerInCab = false;
  int loc1 = rand() % 4;
  int loc2 = rand() % 4;
  while (loc2 == loc1)
    loc2 = rand() % 4;
  pickUpGridPos.x = setCoords(true, loc1, 'x');
  pickUpGridPos.y = setCoords(true, loc1, 'y');
  pickUpPos = getPosFromLocCode(loc1);

  dropOffGridPos.x = setCoords(false, loc2, 'x');
  dropOffGridPos.y = setCoords(false, loc2, 'y');
  dropOffPos = getPosFromLocCode(loc2);

  //	printf("loc1 = %d\n",loc1);
  //	printf("loc2 = %d\n",loc2);
  //	printf("pickUpPos = (%d,%d)\n",pickUpPos.x,pickUpPos.y);
  //	printf("dropOffPos = (%d,%d)\n",dropOffPos.x,dropOffPos.y);

  currPos.x = pickUpPos.x;
  currPos.y = pickUpPos.y;
}

Position Passenger::getPosFromLocCode(int &loc) {
  Position pos;
  switch (loc) { // clockwise direction
  case 0:        // R
    pos.x = 0;
    pos.y = 0;
    break;
  case 1: // G
    pos.x = 4;
    pos.y = 0;
    break;
  case 2: // B
    pos.x = 3;
    pos.y = 4;
    break;
  case 3: // Y
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
  switch (s) {
  case 0:
    isPassengerInCab = false;
    pickUpCode = 'R';
    pickUpPos = getPosFromLocCode(s);
    pickUpGridPos.x =
        (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
    pickUpGridPos.y =
        (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
    break;
  case 1:
    isPassengerInCab = false;
    pickUpCode = 'G';
    pickUpPos = getPosFromLocCode(s);
    pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 +
                      GRID_SIZE * 4 + GRID_SIZE / 2;
    pickUpGridPos.y =
        (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE / 2;
    break;
  case 2:
    isPassengerInCab = false;
    pickUpCode = 'B';
    pickUpPos = getPosFromLocCode(s);
    pickUpGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 +
                      GRID_SIZE * 3 + GRID_SIZE / 2;
    pickUpGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 +
                      GRID_SIZE * 4 + GRID_SIZE / 2;
    break;
  case 3:
    isPassengerInCab = false;
    pickUpCode = 'Y';
    pickUpPos = getPosFromLocCode(s);
    pickUpGridPos.x =
        (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
    pickUpGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 +
                      GRID_SIZE * 4 + GRID_SIZE / 2;
    break;
  }

  switch (d) { // didn't implement check for different s and d, assuming the
               // user invokes correctly
  case 0:
    dropOffCode = 'R';
    dropOffPos = getPosFromLocCode(d);
    dropOffGridPos.x =
        (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
    dropOffGridPos.y =
        (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
    break;
  case 1:
    dropOffCode = 'G';
    dropOffPos = getPosFromLocCode(d);
    dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 +
                       GRID_SIZE * 4 + GRID_SIZE / 2;
    dropOffGridPos.y =
        (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE / 2;
    break;
  case 2:
    dropOffCode = 'B';
    dropOffPos = getPosFromLocCode(d);
    dropOffGridPos.x = (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 +
                       GRID_SIZE * 3 + GRID_SIZE / 2;
    dropOffGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 +
                       GRID_SIZE * 4 + GRID_SIZE / 2;
    break;
  case 3:
    dropOffCode = 'Y';
    dropOffPos = getPosFromLocCode(d);
    dropOffGridPos.x =
        (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
    dropOffGridPos.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 +
                       GRID_SIZE * 4 + GRID_SIZE / 2;
    break;
  }

  currPos.x = pickUpPos.x;
  currPos.y = pickUpPos.y;

} // Problem: source (s) can be inside the taxi, which is not a valid spawn
  // location
// In that case, retain the previous spawn location. s= 0,1,2,3,4 corresponds to
// R,G,B,Y,insideCab

int Passenger::setCoords(bool isPickup, int key, char coord) {
  if (key == 0) {
    if (isPickup)
      pickUpCode = 'R';
    if (!isPickup)
      dropOffCode = 'R';
    if (coord == 'x') {
      return (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
    } else if (coord == 'y')
      return (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE / 2;
  } else if (key == 1) {
    if (isPickup)
      pickUpCode = 'G';
    if (!isPickup)
      dropOffCode = 'G';
    if (coord == 'x')
      return (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE * 4 +
             GRID_SIZE / 2;
    else if (coord == 'y')
      return (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE / 2;
  } else if (key == 2) {
    if (isPickup)
      pickUpCode = 'B';
    if (!isPickup)
      dropOffCode = 'B';
    if (coord == 'x')
      return (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE * 3 +
             GRID_SIZE / 2;
    else if (coord == 'y')
      return (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE * 4 +
             GRID_SIZE / 2;
  } else if (key == 3) {
    if (isPickup)
      pickUpCode = 'Y';
    if (!isPickup)
      dropOffCode = 'Y';
    if (coord == 'x')
      return (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
    else if (coord == 'y')
      return (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE * 4 +
             GRID_SIZE / 2;
  } else {
    printf("Error in key generation (not between 0 to 4)!\n");
  }
  return -1;
}

char Passenger::getCode(int flag) {
  if (flag == 0) {
    return pickUpCode;
  }
  return dropOffCode;
}

int Passenger::getPos(int i, int j) {
  int pos = 0;
  if (i == 0) {
    if (j == 0)
      pos = pickUpPos.x;
    else
      pos = pickUpPos.y;
  } else {
    if (j == 0)
      pos = dropOffPos.x;
    else
      pos = dropOffPos.y;
  }
  return pos;
}

bool Passenger::getPassengerStatus() { return isPassengerInCab; }

void Passenger::setPassengerStatus(bool val) { isPassengerInCab = val; }
