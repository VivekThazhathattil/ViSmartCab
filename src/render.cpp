#include "../include/render.h"
#include "../include/consts.h"
#include "../include/position.h"
#include <unistd.h>

#define IX(x, y) ((x) + (y) * (NUM_GRIDS_X))
#define E(x,y) ((x) + (y * NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES))

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

/*	printf("%d\n",this->env.encodeNEditState(3,1,2,0));
	updateFigure(cab, wall, textR, textG, textB, textY);*/

/*	while(this->window.isOpen()){
		sf::Event e;
                while(window.pollEvent(e))
                {
                        if (e.type == sf::Event::Closed)
                                this->window.close();
                }
                this->window.clear();
		drawNDisplay(parkingLot, cab, wall, textR, textG, textB, textY);
	} */

	this->learn(parkingLot, cab, wall, textR, textG, textB, textY );
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

void Render::setTextProperties(sf::Text& text, sf::Font& font, int fontSize, std::string strg){
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setString(strg);
	text.setFillColor(sf::Color::White);
	if (strg.size() < 3)
        	text.setStyle(sf::Text::Bold);
}

void Render::createRGBYMarkings(sf::Text& R, sf::Text& G ,sf::Text& B, sf::Text& Y, sf::Font& font){
	sf::Vector2f offset = getOffset();

	this->setTextProperties(R, font, 28, "R");
	this->setTextProperties(G, font, 28, "G");
	this->setTextProperties(B, font, 28, "B");
	this->setTextProperties(Y, font, 28, "Y");

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
	int i = this->env.cab.getCurrPosition('x');
	int j = this->env.cab.getCurrPosition('y');
	sf::Vector2f offset = getOffset();
	cab.setOrigin(sf::Vector2f(CAB_X/2, CAB_Y/2));
	cab.setPosition(sf::Vector2f(\
				offset.x + i*GRID_SIZE + GRID_SIZE/2,\
				offset.y + j*GRID_SIZE + GRID_SIZE/2\
				));
	cab.setSize(sf::Vector2f(CAB_X,CAB_Y));
	if(this->env.passenger.getPassengerStatus())
		cab.setFillColor(sf::Color::Green);
	else
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
  sf::Text& Y,\
  sf::Text& info\
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
	this->window.draw(info);

	this->window.draw(cab);
	for (int i = 0; i < wall.size(); i++){
		this->window.draw(wall[i]);
	}
	this->window.display();
}

void Render::learn(\
  		std::vector<sf::RectangleShape>& pl,\
		sf::RectangleShape& cab,\
	       	std::vector<sf::RectangleShape>& wall,\
		sf::Text& textR,\
		sf::Text& textG,\
		sf::Text& textB,\
		sf::Text& textY\
){
	sf::Text info;
	sf::Font font; // for RGBY markings
	if(!font.loadFromFile("res/arial.ttf")){
		printf("Error in loading font from file\n");
	}
	std::srand(time(0));
	int epochs, penalties, reward, score;
	bool done;
	int cabI, cabJ, passengerIdx, destIdx, state, nextState;
	float oldQ, newQ, nextMaxQ;
	int actionCode;
	int iter = this->env.iterator;
	std::string mode;

	for (; iter < NUM_ITERATIONS && this->window.isOpen(); iter++){
		sf::Event e;
                while(window.pollEvent(e))
                {
                        if (e.type == sf::Event::Closed)
                                this->window.close();
                }
		this->env.reset(); //reset our sample space
		updateFigure(cab, wall, textR, textG, textB, textY);
		cabI = this->env.cab.getSpawnPosition('x');
		cabJ = this->env.cab.getSpawnPosition('y');
		if (this->env.passenger.getPassengerStatus())
			passengerIdx = 4;
		else{
			if(this->env.passenger.getCode(0) == 'R')
				passengerIdx = 0;
			else if(this->env.passenger.getCode(0) == 'G')
				passengerIdx = 1;
			else if(this->env.passenger.getCode(0) == 'B')
				passengerIdx = 2;
			else if(this->env.passenger.getCode(0) == 'Y')
				passengerIdx = 3;
		}	
		if(this->env.passenger.getCode(1) == 'R')
			destIdx = 0;
		else if(this->env.passenger.getCode(1) == 'G')
			destIdx = 1;
		else if(this->env.passenger.getCode(1) == 'B')
			destIdx = 2;
		else if(this->env.passenger.getCode(1) == 'Y')
			destIdx = 3;
		state = this->env.encode(cabI, cabJ, passengerIdx, destIdx);
		epochs = 0;
		penalties = 0;
		reward = 0;
		done = false;
		score = 0;
		while(!done){
                	this->window.clear();
			float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
			if(r < EPSILON){
				mode = "Explore";
				actionCode = std::rand()%NUM_ACTIONS; //exploration
			}
			else{
				mode = "Exploit";
				actionCode = this->env.getActionForMaxQValue(state); //exploitation
			}
			this->env.step(actionCode, state, nextState, reward, done);
			oldQ = this->env.qTable[E(state,actionCode)];
			nextMaxQ = this->env.getMaxQForState(state);
			newQ = ((1-ALPHA) * oldQ)+\
			       (ALPHA * (reward + GAMMA * nextMaxQ));
			this->env.qTable[E(state, actionCode)] = newQ;
			if (reward == -10)
				penalties += 1;
			score += reward;
			state = nextState;
			epochs += 1;

			this->env.decode(state, cabI, cabJ, passengerIdx, destIdx);
			std::string inf = "Episode no.      : " + std::to_string((int)(iter+1)) + "\n"+\
					  "Action                : " + this->env.actionCodeToString(actionCode) + "\n"+\
					  "State                 : " + std::to_string((int)state) + "\n" +\
					  "Cab Location    : " + "(" + std::to_string((int)cabI) + "," + std::to_string((int)cabJ) + ")\n"+\
					  "Mode                  : " + mode + "\n" +\
					  "Time spent      : " + std::to_string((int)epochs) + "\n" +\
					  "score                 : " + std::to_string((int)score);
			this->setTextProperties(info, font, 16, inf);
			
//			printf("%d\n",actionCode);
			this->stepFigure(nextState, pl, cab, wall, textR, textG, textB, textY);
			drawNDisplay(pl, cab, wall, textR, textG, textB, textY, info);
			this->env.iterator = iter;
			this->env.saveQTableToFile();
			if(epochs <= 20)
				usleep(0.5 * 1000000);
		}

	}
}

void Render::stepFigure(\
		int nextState,\
		std::vector<sf::RectangleShape>& pl,\
		sf::RectangleShape& cab,\
	       	std::vector<sf::RectangleShape>& wall,\
		sf::Text& textR,\
		sf::Text& textG,\
		sf::Text& textB,\
		sf::Text& textY\
){
	int code = nextState;
	int cabI, cabJ, passengerIdx, destIdx;
	this->env.decode(code, cabI, cabJ, passengerIdx, destIdx);
	this->env.cab.setCurrPosition(cabI, cabJ);
	this->createCab(cab);
//	this->createWall(wall);
//	this->createPassenger(textR, textG, textB, textY);
}
