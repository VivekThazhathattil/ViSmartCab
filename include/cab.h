#include "position.h"

class Cab{
	private:
		Position spawnPos;
		Position currPos;
	public:
		Cab();
		~Cab();
		void performAction();
		void setRandomSpawnPosition();
		int getSpawnPosition(char i);
		void setSpecificSpawnPosition(int x, int y);
};
