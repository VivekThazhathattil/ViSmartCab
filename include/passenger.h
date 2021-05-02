#include "position.h"

class Passenger{
	private:
		Position pickUpPos;
		Position dropOffPos;
		Position currPos;
		bool isPassengerInCab;
	public:
		Passenger();
		~Passenger();
		void updatePassengerPosition(); // if passenger inside the cab, his position will change
		void updatePassengerState(); // whether inside or outside the cab
		void setRandomSpawnPosition();
};
