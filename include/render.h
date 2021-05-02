#include <SFML/Graphics.hpp>
#include <vector>
#include "env.h"

class Render{
	private:
		Env env;
    		sf::RenderWindow window;
	public:
		Render();
		~Render();

		void runSimulation();
		void createParkingLot(std::vector<sf::RectangleShape>& parkingLot);
		void createRGBYMarkings();
		void createCab(sf::RectangleShape& cab);
		void createWall(std::vector<sf::RectangleShape>& wall);
		void createPassenger();
		void drawNDisplay(\
				std::vector<sf::RectangleShape>& pl,\
			       	sf::RectangleShape& cab,\
			       	std::vector<sf::RectangleShape>& wall);
};
