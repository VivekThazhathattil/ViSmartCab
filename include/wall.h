#include "position.h"
#include "consts.h"

class Wall{
	private:
		WallPosition wallPos[NUM_WALLS];
		char getWallType(WallPosition& w);
	public:
		Wall();
		~Wall();

		WallPosition getWallPosition(int num);
		bool checkWallCollision(int& cabI, int&cabJ, int& action); // should check for boundary cases as well
};
