#include "../include/render.h"
#include "../include/consts.h"

#define IX(x, y) ((x) + (y) * (NUM_GRIDS_X))

Render::Render() : window(sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "SMARTCAB in C++!", sf::Style::Close)) {}
Render::~Render() {}

void Render::runSimulation(){
	this->window.setPosition(sf::Vector2i(100,100));

	std::vector<sf::RectangleShape> parkingLot;
	sf::RectangleShape cab;

	createParkingLot(parkingLot);
	createCab(cab);
	while(this->window.isOpen()){
		sf::Event e;
                while(window.pollEvent(e))
                {
                        if (e.type == sf::Event::Closed)
                                this->window.close();
                }
                this->window.clear();
		drawNDisplay(parkingLot, cab);
	}
}

sf::Vector2f getOffset(){
	return sf::Vector2f(\
		(WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2,\
		(WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2\
		);
}

void Render::createParkingLot(std::vector<sf::RectangleShape>& pl){
	sf::Vector2f offset = getOffset();
	sf::RectangleShape rect;
	for (int i = 0; i < NUM_GRIDS_X; i++){
		for (int j = 0; j < NUM_GRIDS_Y; j++){
			rect.setPosition(sf::Vector2f(\
						offset.x + i*GRID_SIZE,\
						offset.y + j*GRID_SIZE\
						));
			rect.setSize(sf::Vector2f(GRID_SIZE,GRID_SIZE));
			rect.setFillColor(sf::Color::Black);
			rect.setOutlineThickness(1);
			rect.setOutlineColor(sf::Color::White);
			pl.push_back(rect);
		}
	}
}

void Render::createCab(sf::RectangleShape& cab){
	int i = this->env.cab.getSpawnPosition('x');
	int j = this->env.cab.getSpawnPosition('y');
	sf::Vector2f offset = getOffset();
	cab.setOrigin(sf::Vector2f(CAB_X/2, CAB_Y/2));
	cab.setPosition(sf::Vector2f(\
				offset.x + i*GRID_SIZE + GRID_SIZE/2,\
				offset.y + j*GRID_SIZE + GRID_SIZE/2\
				));
	cab.setSize(sf::Vector2f(CAB_X,CAB_Y));
	cab.setFillColor(sf::Color::Yellow);
	cab.setOutlineThickness(2);
	cab.setOutlineColor(sf::Color::White);
}

void Render::drawNDisplay(std::vector<sf::RectangleShape>& pl, sf::RectangleShape& cab){
	for (int i = 0; i < NUM_GRIDS_X; i++){
		for (int j = 0; j < NUM_GRIDS_Y; j++){
		this->window.draw(pl[IX(i,j)]);
		}
	}

	this->window.draw(cab);
	this->window.display();
}
