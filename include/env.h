#include "cab.h"
#include "consts.h"
#include "passenger.h"
#include "rewardTable.h"
#include "wall.h"
#include <string>

class Env {
private:
  bool encodeArr[NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES *
                 NUM_DEST_STATES];
  int numPassengerStates;
  int numDestStates;
  int numCabXStates;
  int numCabYStates;
  RewardTable rewardTable[NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES *
                          NUM_DEST_STATES * NUM_ACTIONS];

  void setupEncodeArr();

  void learn(); // learning algorithm (headless)

  int getNextState(int state, int action);
  int getReward(int state, int action);
  bool isDone(int state, int action);
  void saveRewardTableToFile(std::string fileName);

public:
  double qTable[NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES *
                NUM_DEST_STATES * NUM_ACTIONS];

  int iterator;
  Wall wall;
  Cab cab;
  Passenger passenger;
  Position R, G, B, Y;
  int score;
  int currTime;
  Env();
  ~Env();
  void updateScore(int amount);
  int encodeNEditState(int cabI, int cabJ, int passengerIdx,
                       int destIdx); // encode returns the code as well
                                     // as sets the cab spawn and passenger
                                     // spawn locations
  int encode(int cabI, int cabJ, int passengerIdx,
             int destIdx); // only encodes the state
  void decode(int code, int &cabI, int &cabJ, int &passengerIdx, int &destIdx);

  void reset(); // reset the whole world
  void resetQTable();
  void initializeRewardTable();
  void updateQTable();
  int getActionForMaxQValue(int &state);
  void step(int actionCode, int state, int &nextState, int &reward, bool &done);
  double getMaxQForState(int &state);
  std::string actionCodeToString(int &code);
  void saveOtherStats(std::string fileName, int epochs, int score);
  void saveQTableToFile(std::string fileName);
  void getQTableFromFile();
};
