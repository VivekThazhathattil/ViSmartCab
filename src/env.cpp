#include "../include/env.h"
#include "../include/position.h"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <time.h>

//#include <cstdlib>
//#include <cstdio>
#define E(x, y)                                                                \
  ((x) +                                                                       \
   (y * NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES))

Env::Env() {
  R.x = (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
  R.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE / 2;

  G.x = (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE * 4 +
        GRID_SIZE / 2;
  G.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE / 2;

  B.x = (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE * 3 +
        GRID_SIZE / 2;
  B.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE * 4 +
        GRID_SIZE / 2;

  Y.x = (WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2 + GRID_SIZE / 2;
  Y.y = (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2 + GRID_SIZE * 4 +
        GRID_SIZE / 2;

  numPassengerStates = NUM_PASSENGER_STATES;
  numDestStates = NUM_DEST_STATES;
  numCabXStates = NUM_GRIDS_X;
  numCabYStates = NUM_GRIDS_Y;

  setupEncodeArr();
  initializeRewardTable();

  if (std::filesystem::exists("./saveData/qTable.dat")) {
    getQTableFromFile();
  } else {
    resetQTable();
    iterator = 0;
  }
}
Env::~Env() {}

int Env::encodeNEditState(int cabI, int cabJ, int passengerIdx, int destIdx) {
  cab.setSpecificSpawnPosition(cabI, cabJ);
  passenger.setSpecificSpawnAndDropPosition(passengerIdx, destIdx);
  return encode(cabI, cabJ, passengerIdx, destIdx);
}

int Env::encode(int cabI, int cabJ, int passengerIdx, int destIdx) {
  return cabI + numCabXStates * cabJ +
         numCabXStates * numCabYStates * passengerIdx +
         numCabXStates * numCabYStates * numPassengerStates * destIdx;
}

void Env::setupEncodeArr() {
  for (int k = 0; k < numDestStates; k++)
    for (int j = 0; j < numPassengerStates; j++)
      for (int b = 0; b < numCabYStates; b++)
        for (int a = 0; a < numCabXStates; a++)
          encodeArr[a + numCabXStates * b + numCabXStates * numCabYStates * j +
                    numCabXStates * numCabYStates * numPassengerStates * k] = 0;
}

void Env::initializeRewardTable() {
  for (int i = 0;
       i < NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES;
       i++) {
    for (int j = 0; j < NUM_ACTIONS; j++) {
      rewardTable[E(i, j)].probability = 1;
      rewardTable[E(i, j)].nextState = getNextState(i, j);
      rewardTable[E(i, j)].reward = getReward(i, j);
      rewardTable[E(i, j)].done = isDone(i, j);
    }
  }
  passenger.setPassengerStatus(
      false); // getNextState may cause passenger status to vary
  saveRewardTableToFile("./saveData/rewardTable.dat");
}

void Env::resetQTable() {
  for (int i = 0; i < NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES *
                          NUM_DEST_STATES * NUM_ACTIONS;
       i++)
    qTable[i] = 0; // try optimistic initialization later
}

void Env::decode(int code, int &cabI, int &cabJ, int &passengerIdx,
                 int &destIdx) {
  destIdx = (int)(code / (numCabXStates * numCabYStates * numPassengerStates));
  code = code % (numCabXStates * numCabYStates * numPassengerStates);
  passengerIdx = (int)(code / (numCabXStates * numCabYStates));
  code = code % (numCabXStates * numCabYStates);
  cabJ = (int)(code / numCabXStates);
  cabI = code % (numCabXStates);
}

int Env::getNextState(int state, int action) {
  int code = state;
  int cabI, cabJ, passengerIdx, destIdx;
  Position passengerSpawnPos, passengerDestPos;
  decode(code, cabI, cabJ, passengerIdx, destIdx);
  if (passengerIdx != 4)
    passengerSpawnPos = passenger.getPosFromLocCode(passengerIdx);
  passengerDestPos = passenger.getPosFromLocCode(destIdx);

  switch (action) {
  case MOVE_DOWN: // south
    if (!wall.checkWallCollision(cabI, cabJ, action))
      code = encode(cabI, cabJ + 1, passengerIdx, destIdx);
    break;

  case MOVE_UP: // north
    if (!wall.checkWallCollision(cabI, cabJ, action))
      code = encode(cabI, cabJ - 1, passengerIdx, destIdx);
    break;

  case MOVE_RIGHT: // east
    if (!wall.checkWallCollision(cabI, cabJ, action))
      code = encode(cabI + 1, cabJ, passengerIdx, destIdx);
    break;

  case MOVE_LEFT: // west
    if (!wall.checkWallCollision(cabI, cabJ, action))
      code = encode(cabI - 1, cabJ, passengerIdx, destIdx);
    break;

  case PICKUP: // pickup
    if (passengerIdx != 4)
      if (passengerSpawnPos.x == cabI && passengerSpawnPos.y == cabJ &&
          passengerIdx != 4) {
        passenger.tempPosCode[state] = passengerIdx;
        passengerIdx = 4;
        passenger.setPassengerStatus(true);
        code = encode(cabI, cabJ, passengerIdx, destIdx);
      }
    break;
  case DROPOFF: // dropoff
    if (passengerIdx == 4 && passengerDestPos.x == cabI &&
        passengerDestPos.y == cabJ) {
      //				printf("passenger dropped off at
      //(%d,%d)\n",cabI,cabJ);
      passenger.setPassengerStatus(false);
      printf("tempPosCode : %d \n", passenger.tempPosCode[state]);
      code = encode(cabI, cabJ, passenger.tempPosCode[state], destIdx);
    }
    break;
  }
  return code;
}

int Env::getReward(int state, int action) {
  int cabI, cabJ, passengerIdx, destIdx;
  decode(state, cabI, cabJ, passengerIdx, destIdx);
  Position passengerSpawnPos, passengerDestPos;
  if (passengerIdx != 4)
    passengerSpawnPos = passenger.getPosFromLocCode(passengerIdx);
  passengerDestPos = passenger.getPosFromLocCode(destIdx);
  int reward = 0;
  switch (action) {
  case 0: // south

  case 1: // north

  case 2: // east

  case 3: // west
    if (wall.checkWallCollision(cabI, cabJ, action))
      reward = -2;
    else
      reward = -1; // all movements attribute to -1 reward
    break;

  case 4: // pickup
    /* correct pickup deserves positive reinforcement */
    if (passengerIdx != 4) {
      if (passengerSpawnPos.x == cabI && passengerSpawnPos.y == cabJ)
        reward = 10;
      else
        reward = -5;
    } else
      reward = -5; /* wrong pickup needs to be punished */
    break;

  case 5: // dropoff
    if (passengerDestPos.x == cabI && passengerDestPos.y == cabJ &&
        passengerIdx == 4)
      reward = 50;
    else
      reward = -5; /* wrong drop off attempt punished */
    break;
  }
  return reward;
}

bool Env::isDone(int state, int action) {
  int cabI, cabJ, passengerIdx, destIdx;
  decode(state, cabI, cabJ, passengerIdx, destIdx);
  bool flag = false;
  Position passengerDestPos = passenger.getPosFromLocCode(destIdx);
  switch (action) {
  case 0: // south

  case 1: // north

  case 2: // east

  case 3: // west

  case 4: // pickup
    break;

  case 5: // dropoff
    if (passengerDestPos.x == cabI && passengerDestPos.y == cabJ &&
        passengerIdx == 4)
      flag = true;
    break;
  }
  return flag;
}

int Env::getActionForMaxQValue(int &state) {
  /* Looks in the QTable for a particular state and find the action with the max
   * q-value*/
  double maxVal = -1 * (std::numeric_limits<float>::max());
  int jCandidate = 0;
  for (int j = 0; j < NUM_ACTIONS; j++) {
    if (qTable[E(state, j)] >= maxVal) {
      maxVal = qTable[E(state, j)];
      jCandidate = j;
    }
  }
  return jCandidate;
}

void Env::step(int actionCode, int state, int &nextState, int &reward,
               bool &done) {
  int code = state;
  int cabI, cabJ, passengerIdx, destIdx;
  decode(code, cabI, cabJ, passengerIdx, destIdx);

  /*	nextState = getNextState(state, actionCode);
          if (!passenger.getPassengerStatus() && passengerIdx == 4)
                  passenger.setPassengerStatus(true);
          reward = getReward(state, actionCode);
          done = isDone(state, actionCode); */

  nextState = rewardTable[E(state, actionCode)].nextState;
  if (!passenger.getPassengerStatus() && passengerIdx == 4)
    passenger.setPassengerStatus(true);
  reward = rewardTable[E(state, actionCode)].reward;
  done = rewardTable[E(state, actionCode)].done;
}

double Env::getMaxQForState(int &state) {
  double maxVal = -1 * (std::numeric_limits<float>::max());

  for (int j = 0; j < NUM_ACTIONS; j++) {
    if (qTable[E(state, j)] > maxVal) {
      maxVal = qTable[E(state, j)];
    }
  }
  return maxVal;
}

void Env::reset() {
  /* reset cab position, passenger position */
  cab.setRandomSpawnPosition();
  passenger.setRandomSpawnAndDropPosition();
}

std::string Env::actionCodeToString(int &code) {
  std::string text;
  switch (code) {
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

void Env::saveQTableToFile(std::string fileName) {
  std::ofstream qFile;
  qFile.open(fileName, std::fstream::trunc);
  if (!qFile.is_open()) {
    std::cerr << "There was a problem opening the input file!\n";
    exit(1);
  }
  for (int i = 0; i <= NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES *
                           NUM_DEST_STATES * NUM_ACTIONS;
       i++) {
    if (i == 0)
      qFile << iterator << std::endl;
    else {
      if (!std::isinf(qTable[i - 1]))
        qFile << qTable[i - 1] << std::endl;
      else {
        qFile << NEG_LIM << std::endl;
        printf("inf entry encountered at line no: %d while saving QTable to "
               "local file \n",
               i + 1);
      }
    }
  }
  qFile.close();
}

void Env::saveRewardTableToFile(std::string fileName) {
  int cabI0, cabI1, cabJ0, cabJ1, psg_idx0, dest_idx0, psg_idx1, dest_idx1;
  std::ofstream qFile;
  qFile.open(fileName, std::fstream::trunc);
  if (!qFile.is_open()) {
    std::cerr << "There was a problem opening the input file!\n";
    exit(1);
  }
  qFile << "(state,action)   init_cab_pos    next_cab_pos    psg_spawn_idx0    "
           "psg_spawn_idx1    psg_dest_idx0    psg_dest_idx1    reward\n";
  for (int i = 0;
       i < NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES;
       i++)
    for (int j = 0; j < NUM_ACTIONS; j++) {
      decode((i) % (NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES *
                    NUM_DEST_STATES),
             cabI0, cabJ0, psg_idx0, dest_idx0);
      decode(rewardTable[E(i, j)].nextState, cabI1, cabJ1, psg_idx1, dest_idx1);
      qFile << "(" << i << "," << actionCodeToString(j) << ")"
            << "\t (" << cabI0 << "," << cabJ0 << ") ---> (" << cabI1 << ","
            << cabJ1 << ")\t " << psg_idx0 << " ---> " << psg_idx1 << "\t"
            << dest_idx0 << " ---> " << dest_idx1 << " \t "
            << rewardTable[E(i, j)].reward << std::endl;
    }
  qFile.close();
}

void Env::getQTableFromFile() {
  std::ifstream qFile;
  qFile.open("./saveData/qTable.dat", std::fstream::in);
  if (!qFile.is_open()) {
    std::cerr << "There was a problem opening the input file!\n";
    exit(1);
  }
  double num = 0.0;
  int i = 0;
  while (qFile >> num && i <= NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES *
                                  NUM_DEST_STATES * NUM_ACTIONS) {
    if (i == 0)
      iterator = num;
    else {
      if (!std::isinf(num))
        qTable[i - 1] = num;
      else
        qTable[i - 1] = NEG_LIM;
    }
    i++;
  }
}
