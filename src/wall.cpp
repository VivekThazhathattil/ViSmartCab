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
  addWall('h', 1, 0, 4);
  addWall('v', 1, 3, 3);
  addWall('v', 1, 4, 3);
  addWall('h', 1, 2, 4);
  addWall('v', 1, 2, 4);
  addWall('v', 1, 4, 1);
  addWall('h', 1, 4, 4);
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

void Wall::removeWall(int x, int y, char orientation){
    int n = getNumWalls();
    for(int i = 0; i < n; ++i){
        WallPosition wp = wallPos[i];
        if(getWallType(wp) == orientation){
            if(orientation == 'V'){
                if(wp.y0 <= y && y <= wp.y1){
                    std::vector<WallPosition>::iterator it = wallPos.begin() + i;
                    wallPos.erase(it);
                    printf("Wall removed at (%d,%d)", x, y);
                    break;
                }
            }
            else
                if(wp.x0 <= x && x <= wp.x1){
                    std::vector<WallPosition>::iterator it = wallPos.begin() + i;
                    wallPos.erase(it);
                    printf("Wall removed at (%d,%d)", x, y);
                    break;
                }
        }
    }
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
            //printf("move-down Collision!\n");
            break;
        }
        else if(action == MOVE_UP && wallPos.y0 == cabJ
                && wallPos.x0 <= cabI && cabI < wallPos.x1){
            flag = true;
            //printf("move-up Collision!\n");
            break;
        }

    }

    else if(getWallType(wallPos) == 'V' && 
            (action == MOVE_LEFT || action == MOVE_RIGHT)){
        if(action == MOVE_LEFT && wallPos.x0 == cabI
                && wallPos.y0 <= cabJ && cabJ < wallPos.y1){
            flag = true;
            //printf("move-left Collision!\n");
            break;
        }
        else if(action == MOVE_RIGHT && wallPos.x0 == cabI + 1
                && wallPos.y0 <= cabJ && cabJ < wallPos.y1){
            flag = true;
            //printf("move-right Collision!\n");
            break;
        }
    }
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

bool Wall::wallAlreadyExists(int x, int y, char orientation){
    for(int i = 0; i < wallPos.size(); ++i){
        WallPosition wp = wallPos[i];
        if(getWallType(wp) == 'H' && orientation == 'h'){
            if(y == wp.y0 && x == wp.x0){
                printf("Horizontal wall already exists at (%d,%d)", x, y);
                return true;
            }
        }
        else if(getWallType(wp) == 'V' && orientation == 'v')
            if(x == wp.x0 && y == wp.y0){
                printf("Vertical wall already exists at (%d,%d)", x, y);
                return true;
            }
    }
    return false;
}

void Wall::buildWall(int x, int y){
  int tolerance, xTol, xTol0, xTol1, yTol, yTol0, yTol1;
  tolerance = TOLERANCE; // pixels
                         
  xTol0 = x % GRID_SIZE;
  xTol1 = GRID_SIZE - (x % GRID_SIZE);
  xTol = xTol0 < xTol1 ? xTol0 : xTol1;

  yTol0 = y % GRID_SIZE;
  yTol1 = GRID_SIZE - (y % GRID_SIZE);
  yTol = yTol0 < yTol1 ? yTol0 : yTol1;

  printf("<<<<<<<<xTol0 = %d, xTol1 = %d, xTol = %d\n", xTol0, xTol1, xTol);
  printf("---mouse(x,y) = (%d, %d)\n ---mouse(xTol, yTol) = (%d,%d)\n TOLERANCE=%d", x, y, xTol, yTol, tolerance);

  if(xTol <= yTol && xTol < tolerance){
    x = x / GRID_SIZE;
    y = y / GRID_SIZE;
    if(!wallAlreadyExists(x, y, 'v')){
        addWall('v', 1, x, y);
        printf("Wall added at (%d,%d)", x, y);
    }
  }
  else if(yTol <= xTol && yTol < tolerance){
    y = y / GRID_SIZE;
    x = x / GRID_SIZE;
    if(!wallAlreadyExists(x, y, 'h')){
        addWall('h', 1, x, y);
        printf("Wall added at (%d,%d)", x, y);
    }
  }
  return;
}

void Wall::destroyWall(){
  if(!wallPos.empty())
    wallPos.pop_back();
  return;
}
