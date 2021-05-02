#include "position.h"
class Wall{
	private:
		WallPosition wallPos;
	public:
		Wall();
		~Wall();

		void checkWallCollision();
};
