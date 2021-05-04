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
		void updateFigure(\
				sf::RectangleShape& cab,\
			       	std::vector<sf::RectangleShape>& wall,\
				sf::Text& textR,\
				sf::Text& textG,\
				sf::Text& textB,\
				sf::Text& textY\
				);
		void resetTextColor(sf::Text& R, sf::Text& G, sf::Text& B, sf::Text& Y);
		void setTextProperties(sf::Text& text, sf::Font& font, int fontSize, std::string strg);
		void createParkingLot(std::vector<sf::RectangleShape>& parkingLot);
		void createRGBYMarkings(sf::Text& R, sf::Text& G ,sf::Text& B, sf::Text& Y, sf::Font& font);
		void createCab(sf::RectangleShape& cab);
		void createWall(std::vector<sf::RectangleShape>& wall);
		void createPassenger( sf::Text& R, sf::Text& G, sf::Text& B, sf::Text& Y);
		void drawNDisplay(\
				std::vector<sf::RectangleShape>& pl,\
			       	sf::RectangleShape& cab,\
			       	std::vector<sf::RectangleShape>& wall,\
  				sf::Text& R,\
  				sf::Text& G,\
  				sf::Text& B,\
  				sf::Text& Y,\
  				sf::Text& info\
				);
		void stepFigure(\
				int nextState,\
				std::vector<sf::RectangleShape>& pl,\
			       	sf::RectangleShape& cab,\
			       	std::vector<sf::RectangleShape>& wall,\
  				sf::Text& R,\
  				sf::Text& G,\
  				sf::Text& B,\
  				sf::Text& Y\
				);
		void learn(\
  				std::vector<sf::RectangleShape>& pl,\
			       	sf::RectangleShape& cab,\
			       	std::vector<sf::RectangleShape>& wall,\
  				sf::Text& R,\
  				sf::Text& G,\
  				sf::Text& B,\
  				sf::Text& Y\
				);
};
