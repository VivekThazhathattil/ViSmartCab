#include "../include/wall.h"

//#include <cstdio>
//#include <cstdlib>
//#include <string>
//#include <stdio.h>
//#include <iostream>

Wall::Wall() {
  // manually set all the walls : Assuming NUM_WALLS = 5
  // set the coords in top-down order for horizontal walls and left-right order
  // for vertical walls
  //
  addWall(0, 'v', 1, 0, 3);
  addWall(1, 'v', 1, 0, 4);
  addWall(2, 'v', 1, 2, 3);
  addWall(3, 'v', 1, 2, 4);
  addWall(4, 'v', 1, 1, 0);
}
Wall::~Wall() {}

void Wall::addWall(int wallIdx, char orientation, int length, int originX, int originY) {
    wallPos[wallIdx].x0 = originX;
    wallPos[wallIdx].y0 = originY;

    if(orientation == 'v'){
        wallPos[wallIdx].x1 = originX + length;
        wallPos[wallIdx].y1 = originY;
    }

    else{
        wallPos[wallIdx].x1 = originX;
        wallPos[wallIdx].y1 = originY + length;
    }
    return;
}

WallPosition Wall::getWallPosition(int num) { return wallPos[num]; }
bool Wall::checkWallCollision(int &cabI, int &cabJ, int &action) {
  bool flag = false;
  /* The four boundary walls of the parking lot */
  if ((cabI == 0 && action == 3) || (cabI == NUM_GRIDS_X - 1 && action == 2) ||
      (cabJ == 0 && action == 1) || (cabJ == NUM_GRIDS_Y - 1 && action == 0)) {
    flag = true;
  }

  /* Other walls */
  WallPosition wallPos;
  for (int i = 0; i < NUM_WALLS; i++) {
    wallPos = getWallPosition(i);
    if (wallPos.x0 == cabI && wallPos.y0 == cabJ) {
      if (getWallType(wallPos) == 'H') { // horizontal wall
        if (action == 0)
          flag = true;
      } else if (getWallType(wallPos) == 'V') { // vertical wall
        if (action == 2) {
          //					std::cout<<"wall collision ("<<
          // std::to_string(int(cabI)) << "," << std::to_string(int(cabJ))<< ")
          //"<< std::to_string(int(action))<<std::endl;
          flag = true;
        }
      }
    }

    else if (wallPos.x1 == cabI && wallPos.y1 == cabJ) {
      if (getWallType(wallPos) == 'H') { // horizontal wall
        if (action == 1)
          flag = true;
      } else if (getWallType(wallPos) == 'V') { // vertical wall
        if (action == 3) {
          //					std::cout<<"wall collision ("<<
          // std::to_string(int(cabI)) << "," << std::to_string(int(cabJ))<< ")
          //"<< std::to_string(int(action))<<std::endl;
          flag = true;
        }
      }
    }
  }

  return flag;
}

char Wall::getWallType(WallPosition &w) {
  if (w.y0 == w.y1)
    return 'V';
  return 'H';
}
