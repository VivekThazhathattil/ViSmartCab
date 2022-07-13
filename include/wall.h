#include "consts.h"
#include "position.h"
#include <vector>

class Wall {
private:
  WallPosition wallPos[NUM_WALLS];
  //std::vector<WallPosition> wallPos(NUM_WALLS);
  char getWallType(WallPosition &w);

public:
  Wall();
  ~Wall();

  WallPosition getWallPosition(int num);
  void addWall(int wallIdx, char orientation, int length, int originX, int originY);
  bool
  checkWallCollision(int &cabI, int &cabJ,
                     int &action); // should check for boundary cases as well
};
