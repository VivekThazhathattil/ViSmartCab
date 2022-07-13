#include "position.h"

class Cab {
private:
  Position spawnPos;
  Position currPos;
  int moveDirection;

public:
  Cab();
  ~Cab();
  void setRandomSpawnPosition();
  int getSpawnPosition(char i);
  void setSpecificSpawnPosition(int x, int y);
  int getCurrPosition(char i);
  void setCurrPosition(int &x, int &y);
  int getOrientation(void);
  void setOrientation(const int orientation);
};
