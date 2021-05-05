#include "position.h"
#include "consts.h"

class Passenger{
	private:
		Position pickUpGridPos; // in pixels
		Position dropOffGridPos; // in pixels

		Position pickUpPos;; // in grid pts
		Position dropOffPos; // in grid pts
		Position currPos; // in grid pts
		char pickUpCode; // one of R,G,B,Y
		char dropOffCode; // one of R,G,B,y
		bool isPassengerInCab;

		int setCoords(bool isPickup, int key, char coord);
	public:
		int tempPosCode; // temporary variable to hold the spawn position code when passengerIdx is set to 4

		Passenger();
		~Passenger();
		void updatePassengerPosition(); // if passenger inside the cab, his position will change
		void updatePassengerState(); // whether inside or outside the cab
		void setRandomSpawnAndDropPosition();
		void setSpecificSpawnAndDropPosition(int s, int d);
		char getCode(int flag); // flag = 0 -> pickup, flag = 1 -> dropoff
		int getPos(int i, int j); // i : 0=pickUpPos 1=dropOffPos, j: 0=x 1=y
		bool getPassengerStatus(); // is inside the cab or not
		void setPassengerStatus(bool val);
		Position getPosFromLocCode(int& loc);
};

int setCoords(int key, char coord);
