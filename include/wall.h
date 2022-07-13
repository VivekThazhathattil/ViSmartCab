#include "consts.h"
#include "position.h"
#include <vector>

class Wall {
private:
  std::vector<WallPosition> wallPos;
  char getWallType(WallPosition &w);

public:
  Wall();
  ~Wall();

  WallPosition getWallPosition(int num);
  void addWall(char orientation, int length, int originX, int originY);
  void removeWall(int x, int y, char orientation);
  int getNumWalls();
  void buildWall(int x, int y);
  void destroyWall(int x, int y);
  bool wallAlreadyExists(int x, int y, char orientation);
  bool
  checkWallCollision(int &cabI, int &cabJ,
                     int &action); // should check for boundary cases as well
};
