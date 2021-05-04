#include "../include/wall.h"

//#include <cstdio>
//#include <cstdlib>
//#include <string>
//#include <stdio.h>
//#include <iostream>

Wall::Wall() {
	// manually set all the walls : Assuming NUM_WALLS = 5
	// set the coords in top-down order for horizontal walls and left-right order for vertical walls
	 this->wallPos[0].x0 = 0; 
	 this->wallPos[0].y0 = 3; 
	 this->wallPos[0].x1 = 1; 
	 this->wallPos[0].y1 = 3; 
	
	 this->wallPos[1].x0 = 0;
	 this->wallPos[1].y0 = 4;
	 this->wallPos[1].x1 = 1;
	 this->wallPos[1].y1 = 4;
	
	 this->wallPos[2].x0 = 2;
	 this->wallPos[2].y0 = 3;
	 this->wallPos[2].x1 = 3;
	 this->wallPos[2].y1 = 3;
	
	 this->wallPos[3].x0 = 2;
	 this->wallPos[3].y0 = 4;
	 this->wallPos[3].x1 = 3;
	 this->wallPos[3].y1 = 4;
	
	 this->wallPos[4].x0 = 1;
	 this->wallPos[4].y0 = 0;
	 this->wallPos[4].x1 = 2;
	 this->wallPos[4].y1 = 0;
}
Wall::~Wall() {}

WallPosition Wall::getWallPosition(int num){
	return this->wallPos[num];
}
bool Wall::checkWallCollision(int& cabI, int&cabJ, int& action){
	bool flag = false;
	/* The four boundary walls of the parking lot */
	if (\
		(cabI == 0 && action == 3) ||\
		(cabI == NUM_GRIDS_X - 1 && action == 2) ||\
		(cabJ == 0 && action == 1) ||\
		(cabJ == NUM_GRIDS_Y -1 && action == 0)
	){
		flag = true;
	}

	/* Other walls */
	WallPosition wallPos;
	for (int i = 0; i < NUM_WALLS; i++){
		wallPos = this->getWallPosition(i);
		if (wallPos.x0 == cabI && wallPos.y0 == cabJ){
			if (this->getWallType(wallPos) == 'H'){ // horizontal wall
				if (action == 0)
					flag = true;
			}
			else if (this->getWallType(wallPos) == 'V'){ // vertical wall
				if ( action == 2){
//					std::cout<<"wall collision ("<< std::to_string(int(cabI)) << "," << std::to_string(int(cabJ))<< ") "<< std::to_string(int(action))<<std::endl;
					flag = true;
				}
	
			}
		}

		else if (wallPos.x1 == cabI && wallPos.y1 == cabJ){
			if (this->getWallType(wallPos) == 'H'){ // horizontal wall
				if (action == 1)
					flag = true;
			}
			else if (this->getWallType(wallPos) == 'V'){ // vertical wall
				if (action == 3){
//					std::cout<<"wall collision ("<< std::to_string(int(cabI)) << "," << std::to_string(int(cabJ))<< ") "<< std::to_string(int(action))<<std::endl;
					flag = true;
				}
			}
		}
	}

	return flag;
}

char Wall::getWallType(WallPosition& w){
	if(w.y0 == w.y1)
		return 'V';
	return 'H';
}	











