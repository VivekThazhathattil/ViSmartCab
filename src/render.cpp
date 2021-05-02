#include "../include/render.h"
#include "../include/consts.h"

#define IX(x, y) ((x) + (y) * (NUM_GRIDS_X))

Render::Render() : window(sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "SMARTCAB in C++!", sf::Style::Close)) {}
Render::~Render() {}

void Render::runSimulation(){
	this->window.setPosition(sf::Vector2i(100,100));
	std::vector<sf::RectangleShape> parkingLot;
	createParkingLot(parkingLot);
	while(this->window.isOpen()){
		sf::Event e;
                while(window.pollEvent(e))
                {
                        if (e.type == sf::Event::Closed)
                                this->window.close();
                }
                this->window.clear();
		drawNDisplay(parkingLot);
	}
}

void Render::createParkingLot(std::vector<sf::RectangleShape>& pl){
	sf::Vector2f offset = sf::Vector2f(\
			(WINDOW_SIZE_X - NUM_GRIDS_X*GRID_SIZE)/2,\
			(WINDOW_SIZE_Y - NUM_GRIDS_Y*GRID_SIZE)/2\
			);
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

void Render::drawNDisplay(std::vector<sf::RectangleShape>& pl){
	for (int i = 0; i < NUM_GRIDS_X; i++){
		for (int j = 0; j < NUM_GRIDS_Y; j++){
		this->window.draw(pl[IX(i,j)]);
		}
	}
	this->window.display();
}
