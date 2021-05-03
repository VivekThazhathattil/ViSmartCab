#include "position.h"
#include "consts.h"

class Wall{
	private:
		WallPosition wallPos[NUM_WALLS];
		WallPosition getWallPosition(int num);
	public:
		Wall();
		~Wall();

		bool checkWallCollision(int& cabI, int&cabJ, int& action); // should check for boundary cases as well
};
