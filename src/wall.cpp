#include "../include/wall.h"

Wall::Wall() {
	// manually set all the walls : Assuming NUM_WALLS = 5
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
