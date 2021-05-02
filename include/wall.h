#include "position.h"
#include "consts.h"

class Wall{
	private:
		WallPosition wallPos[NUM_WALLS];
	public:
		Wall();
		~Wall();

		void checkWallCollision();
		WallPosition getWallPosition(int num);
};
