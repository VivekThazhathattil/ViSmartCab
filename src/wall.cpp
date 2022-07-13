#include "../include/wall.h"

//#include <cstdio>
//#include <cstdlib>
//#include <string>
#include <stdio.h>
//#include <iostream>

Wall::Wall() {
  // set the coords in top-down order for horizontal walls and left-right order
  // for vertical walls
  //
  addWall('h', 4, 0, 1);
  addWall('h', 3, 0, 2);
  addWall('h', 2, 0, 3);
  addWall('h', 1, 0, 4);
  addWall('v', 1, 3, 3);
  addWall('v', 1, 4, 3);
  addWall('h', 1, 2, 4);
  //addWall(3, 'h', 1, 2, 4);
  //addWall(4, 'h', 1, 1, 0);
}
Wall::~Wall() {}

void Wall::addWall(char orientation, int length, int originX, int originY) {
    WallPosition wp;

    wp.x0 = originX;
    wp.y0 = originY;

    if(orientation == 'v'){
        wp.x1 = originX;
        wp.y1 = originY + length;
    }

    else{
        wp.x1 = originX  + length;
        wp.y1 = originY;
    }
    wallPos.push_back(wp);
    return;
}

WallPosition Wall::getWallPosition(int num) {
    return wallPos[num]; 
}

bool Wall::checkWallCollision(int &cabI, int &cabJ, int &action) {
  bool flag = false;
  /* The four boundary walls of the parking lot */
  if ((cabI == 0 && action == MOVE_LEFT) || (cabI == NUM_GRIDS_X - 1 && action == MOVE_RIGHT) ||
      (cabJ == 0 && action == MOVE_UP) || (cabJ == NUM_GRIDS_Y - 1 && action == MOVE_DOWN)) {
      return true;
  }

  /* Other walls */
  WallPosition wallPos;
  for (int i = 0; i < getNumWalls(); i++) {
    wallPos = getWallPosition(i);
    if(getWallType(wallPos) == 'H' && 
            (action == MOVE_DOWN || action == MOVE_UP)){
        if(action == MOVE_DOWN && wallPos.y0 == cabJ + 1 
                && (wallPos.x0 <= cabI && cabI < wallPos.x1)){
            flag = true;
            printf("move-down Collision!\n");
            break;
        }
        else if(action == MOVE_UP && wallPos.y0 == cabJ
                && wallPos.x0 <= cabI && cabI < wallPos.x1){
            flag = true;
            printf("move-up Collision!\n");
            break;
        }

    }

    else if(getWallType(wallPos) == 'V' && 
            (action == MOVE_LEFT || action == MOVE_RIGHT)){
        if(action == MOVE_LEFT && wallPos.x0 == cabI
                && wallPos.y0 <= cabJ && cabJ < wallPos.y1){
            flag = true;
            printf("move-left Collision!\n");
            break;
        }
        else if(action == MOVE_RIGHT && wallPos.x0 == cabI + 1
                && wallPos.y0 <= cabJ && cabJ < wallPos.y1){
            flag = true;
            printf("move-right Collision!\n");
            break;
        }
    }

    /*
    if (wallPos.x0 == cabI && wallPos.y0 == cabJ) {
      if (getWallType(wallPos) == 'H') { // horizontal wall
        if (action == MOVE_DOWN){
          flag = true;
          break;
        }
      } else if (getWallType(wallPos) == 'V') { // vertical wall
        if (action == MOVE_RIGHT) {
          //					std::cout<<"wall collision ("<<
          // std::to_string(int(cabI)) << "," << std::to_string(int(cabJ))<< ")
          //"<< std::to_string(int(action))<<std::endl;
          flag = true;
          break;
        }
      }
    }

    else if (wallPos.x1 == cabI && wallPos.y1 == cabJ) {
      if (getWallType(wallPos) == 'H') { // horizontal wall
        if (action == MOVE_UP){
          flag = true;
          break;
        }
      } else if (getWallType(wallPos) == 'V') { // vertical wall
        if (action == MOVE_LEFT) {
          //					std::cout<<"wall collision ("<<
          // std::to_string(int(cabI)) << "," << std::to_string(int(cabJ))<< ")
          //"<< std::to_string(int(action))<<std::endl;
          flag = true;
          break;
        }
      }
    }
    */
  }
  return flag;
}

int Wall::getNumWalls(){
    return wallPos.size();
}

char Wall::getWallType(WallPosition &w) {
  if (w.y0 == w.y1)
    return 'H';
  return 'V';
}
