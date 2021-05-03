#include "position.h"
#include "consts.h"

class Passenger{
	private:
		Position pickUpPos;
		Position dropOffPos;
		Position currPos;
		char pickUpCode; // one of R,G,B,Y
		char dropOffCode; // one of R,G,B,y
		bool isPassengerInCab;

		int setCoords(bool isPickup, int key, char coord);
	public:
		Passenger();
		~Passenger();
		void updatePassengerPosition(); // if passenger inside the cab, his position will change
		void updatePassengerState(); // whether inside or outside the cab
		void setRandomSpawnAndDropPosition();
		void setSpecificSpawnAndDropPosition(int s, int d);
		char getCode(int flag); // flag = 0 -> pickup, flag = 1 -> dropoff
};

int setCoords(int key, char coord);
