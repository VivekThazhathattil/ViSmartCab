#include "../include/render.h"
#include "../include/consts.h"
#include "../include/position.h"

#define IX(x, y) ((x) + (y) * (NUM_GRIDS_X))

Render::Render() : window(sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "SMARTCAB in C++!", sf::Style::Close)) {}
Render::~Render() {}

void Render::runSimulation(){
	this->window.setPosition(sf::Vector2i(100,100));

	std::vector<sf::RectangleShape> parkingLot;
	sf::RectangleShape cab;
	std::vector<sf::RectangleShape> wall;

	sf::Font font; // for RGBY markings
	if(!font.loadFromFile("res/arial.ttf")){
		printf("Error in loading font from file\n");
	}
	sf::Text textR;
	sf::Text textG;
	sf::Text textB;
	sf::Text textY;

	createParkingLot(parkingLot);
	createRGBYMarkings(textR, textG, textB, textY, font);
	updateFigure(cab, wall, textR, textG, textB, textY);

	printf("%d",this->env.encode(3,1,2,0));
	updateFigure(cab, wall, textR, textG, textB, textY);

	while(this->window.isOpen()){
		sf::Event e;
                while(window.pollEvent(e))
                {
                        if (e.type == sf::Event::Closed)
                                this->window.close();
                }
                this->window.clear();
		drawNDisplay(parkingLot, cab, wall, textR, textG, textB, textY);
	}
}

void Render::updateFigure(\
		sf::RectangleShape& cab,\
	       	std::vector<sf::RectangleShape>& wall,\
		sf::Text& textR,\
		sf::Text& textG,\
		sf::Text& textB,\
		sf::Text& textY\

){
	createCab(cab);
	createWall(wall);
	createPassenger(textR, textG, textB, textY);
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

void Render::createRGBYMarkings(sf::Text& R, sf::Text& G ,sf::Text& B, sf::Text& Y, sf::Font& font){
	sf::Vector2f offset = getOffset();

	R.setFont(font);	
	G.setFont(font);	
	B.setFont(font);	
	Y.setFont(font);	

	R.setString("R");
	G.setString("G");
	B.setString("B");
	Y.setString("Y");

	R.setCharacterSize(28);
	G.setCharacterSize(28);
	B.setCharacterSize(28);
	Y.setCharacterSize(28);

	R.setFillColor(sf::Color::White);
	G.setFillColor(sf::Color::White);
	B.setFillColor(sf::Color::White);
	Y.setFillColor(sf::Color::White);

        R.setStyle(sf::Text::Bold);
        G.setStyle(sf::Text::Bold);
        B.setStyle(sf::Text::Bold);
        Y.setStyle(sf::Text::Bold);


	R.setOrigin(\
			R.getGlobalBounds().width/2,\
			R.getGlobalBounds().height/2\
		   );
	G.setOrigin(\
			G.getGlobalBounds().width/2,\
			G.getGlobalBounds().height/2\
		   );
	B.setOrigin(\
			B.getGlobalBounds().width/2,\
			B.getGlobalBounds().height/2\
		   );
	Y.setOrigin(\
			Y.getGlobalBounds().width/2,\
			Y.getGlobalBounds().height/2\
		   );

	R.setPosition(this->env.R.x, this->env.R.y);
	G.setPosition(this->env.G.x, this->env.G.y);
	B.setPosition(this->env.B.x, this->env.B.y);
	Y.setPosition(this->env.Y.x, this->env.Y.y);
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

void Render::createWall(std::vector<sf::RectangleShape>& wall){
	WallPosition wp;
	sf::RectangleShape rect;
	sf::Vector2f offset = getOffset();
	for (int i = 0; i < NUM_WALLS; i++){
		wp = this->env.wall.getWallPosition(i);
		if(wp.x0 == wp.x1) // horizontal wall
		{ 
			rect.setOrigin(sf::Vector2f(0, WALL_Y/2));
			rect.setPosition(sf::Vector2f(\
						offset.x + GRID_SIZE*wp.x1,\
						offset.y + GRID_SIZE*wp.y1\
						));
			rect.setSize(sf::Vector2f(GRID_SIZE, WALL_Y));
			rect.setFillColor(sf::Color::Blue);
			rect.setOutlineThickness(1);
			rect.setOutlineColor(sf::Color::White);
			wall.push_back(rect);
		}
		else if (wp.y0 == wp.y1) // vertical wall
		{
			rect.setOrigin(sf::Vector2f(WALL_X/2, 0));
			rect.setPosition(sf::Vector2f(\
						offset.x + GRID_SIZE*wp.x1,\
						offset.y + GRID_SIZE*wp.y1\
						));
			rect.setSize(sf::Vector2f(WALL_X, GRID_SIZE));
			rect.setFillColor(sf::Color::Blue);
			rect.setOutlineThickness(1);
			rect.setOutlineColor(sf::Color::White);
			wall.push_back(rect);
		}
	}
}

void Render::resetTextColor(sf::Text& R, sf::Text& G, sf::Text& B, sf::Text& Y){
	R.setFillColor(sf::Color::White);
	G.setFillColor(sf::Color::White);
	B.setFillColor(sf::Color::White);
	Y.setFillColor(sf::Color::White);
}

void Render::createPassenger( sf::Text& R, sf::Text& G, sf::Text& B, sf::Text& Y){

	this->resetTextColor(R,G,B,Y);
	switch(this->env.passenger.getCode(0)){
		case 'R':
			R.setFillColor(sf::Color::Magenta);
			break;
		case 'G':
			G.setFillColor(sf::Color::Magenta);
			break;
		case 'B':
			B.setFillColor(sf::Color::Magenta);
			break;
		case 'Y':
			Y.setFillColor(sf::Color::Magenta);
			break;
	}	
	switch(this->env.passenger.getCode(1)){
		case 'R':
			R.setFillColor(sf::Color::Cyan);
			break;
		case 'G':
			G.setFillColor(sf::Color::Cyan);
			break;
		case 'B':
			B.setFillColor(sf::Color::Cyan);
			break;
		case 'Y':
			Y.setFillColor(sf::Color::Cyan);
			break;
	}	
}

void Render::drawNDisplay(\
  std::vector<sf::RectangleShape>& pl,\
  sf::RectangleShape& cab,\
  std::vector<sf::RectangleShape>& wall,\
  sf::Text& R,\
  sf::Text& G,\
  sf::Text& B,\
  sf::Text& Y\
){

	for (int i = 0; i < NUM_GRIDS_X; i++){
		for (int j = 0; j < NUM_GRIDS_Y; j++){
		this->window.draw(pl[IX(i,j)]);
		}
	}

	this->window.draw(R);
	this->window.draw(G);
	this->window.draw(B);
	this->window.draw(Y);

	this->window.draw(cab);
	for (int i = 0; i < wall.size(); i++){
		this->window.draw(wall[i]);
	}
	this->window.display();
}
