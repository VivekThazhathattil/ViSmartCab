#include "../include/render.h"
#include "../include/consts.h"
#include "../include/position.h"
#include <unistd.h>
#include <cstdlib>

#define IX(x, y) ((x) + (y) * (NUM_GRIDS_X))
#define E(x, y)                                                                \
  ((x) +                                                                       \
   (y * NUM_GRIDS_X * NUM_GRIDS_Y * NUM_PASSENGER_STATES * NUM_DEST_STATES))

Render::Render()
    : window(sf::RenderWindow(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y),
                              "SMARTCAB in C++!", sf::Style::Close)) {
  std::srand(time(0));
  if (!parkingLotTexture.loadFromFile("res/green.png")) {
    printf("error loading parkingLotTexture from file\n");
  }
  if (!cabTexture.loadFromFile("res/cab.png")) {
    printf("error loading cab texture from file\n");
  }
  if (!wallTexture.loadFromFile("res/walls.jpg")) {
    printf("error loading wall texture from file\n");
  }
  if (!srcTexture.loadFromFile("res/passenger_entry.png")) {
    printf("error loading passenger_entry texture from file\n");
  }
  if (!destTexture.loadFromFile("res/passenger_exit.png")) {
    printf("error loading passenger_exit texture from file\n");
  }
  srcBlimp.setTexture(&srcTexture, true);
  destBlimp.setTexture(&destTexture, true);
}
Render::~Render() {}

void Render::runSimulation() {
  window.setPosition(sf::Vector2i(100, 100));

  std::vector<sf::RectangleShape> parkingLot;
  sf::RectangleShape cab;
  cab.setTexture(&cabTexture, true);
  std::vector<sf::RectangleShape> wall;

  sf::Font font; // for RGBY markings
  if (!font.loadFromFile("res/arial.ttf")) {
    printf("Error in loading font from file\n");
  }
  sf::Text textR;
  sf::Text textG;
  sf::Text textB;
  sf::Text textY;

  createParkingLot(parkingLot);
  createRGBYMarkings(textR, textG, textB, textY, font);
  updateFigure(cab, wall, textR, textG, textB, textY);
  learn(parkingLot, cab, wall, textR, textG, textB, textY);
}

void Render::updateFigure(sf::RectangleShape &cab,
                          std::vector<sf::RectangleShape> &wall,
                          sf::Text &textR, sf::Text &textG, sf::Text &textB,
                          sf::Text &textY

) {
  createCab(cab);
  createWall(wall);
  createPassenger(textR, textG, textB, textY);
}

sf::Vector2f getOffset() {
  return sf::Vector2f((WINDOW_SIZE_X - NUM_GRIDS_X * GRID_SIZE) / 2,
                      (WINDOW_SIZE_Y - NUM_GRIDS_Y * GRID_SIZE) / 2);
}

void Render::createParkingLot(std::vector<sf::RectangleShape> &pl) {
  sf::Vector2f offset = getOffset();
  sf::RectangleShape rect;
  for (int i = 0; i < NUM_GRIDS_X; i++) {
    for (int j = 0; j < NUM_GRIDS_Y; j++) {
      rect.setPosition(
          sf::Vector2f(offset.x + i * GRID_SIZE, offset.y + j * GRID_SIZE));
      rect.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
      //			rect.setFillColor(sf::Color::Black);
      rect.setOutlineThickness(2);
      rect.setOutlineColor(sf::Color::Black);
      rect.setTexture(&parkingLotTexture);
      pl.push_back(rect);
    }
  }
}

void Render::setTextProperties(sf::Text &text, sf::Font &font, int fontSize,
                               std::string strg) {
  text.setFont(font);
  text.setCharacterSize(fontSize);
  text.setString(strg);
  text.setFillColor(sf::Color::White);
  if (strg.size() < 3)
    text.setStyle(sf::Text::Bold);
}

void Render::createRGBYMarkings(sf::Text &R, sf::Text &G, sf::Text &B,
                                sf::Text &Y, sf::Font &font) {
  sf::Vector2f offset = getOffset();

  setTextProperties(R, font, 28, "R");
  setTextProperties(G, font, 28, "G");
  setTextProperties(B, font, 28, "B");
  setTextProperties(Y, font, 28, "Y");

  R.setOrigin(R.getGlobalBounds().width / 2, R.getGlobalBounds().height / 2);
  G.setOrigin(G.getGlobalBounds().width / 2, G.getGlobalBounds().height / 2);
  B.setOrigin(B.getGlobalBounds().width / 2, B.getGlobalBounds().height / 2);
  Y.setOrigin(Y.getGlobalBounds().width / 2, Y.getGlobalBounds().height / 2);

  R.setPosition(env.R.x, env.R.y);
  G.setPosition(env.G.x, env.G.y);
  B.setPosition(env.B.x, env.B.y);
  Y.setPosition(env.Y.x, env.Y.y);
}

void Render::createCab(sf::RectangleShape &cab) {
  int i = env.cab.getCurrPosition('x');
  int j = env.cab.getCurrPosition('y');
  int orientation = env.cab.getOrientation();

  sf::Vector2f offset = getOffset();
  cab.setOrigin(sf::Vector2f(CAB_X / 2, CAB_Y / 2));
  cab.setPosition(sf::Vector2f(offset.x + i * GRID_SIZE + GRID_SIZE / 2,
                               offset.y + j * GRID_SIZE + GRID_SIZE / 2));
  cab.setSize(sf::Vector2f(CAB_X, CAB_Y));
  if (env.passenger.getPassengerStatus()){
    srcBlimp.setPosition(sf::Vector2f(offset.x + i * GRID_SIZE + GRID_SIZE / 2,
                               offset.y + j * GRID_SIZE + GRID_SIZE / 2));
    cab.setFillColor(sf::Color(0, 255, 0, 255));
  }
  else
    cab.setFillColor(sf::Color::White);
  cab.setScale(2.5, 1.0);
  cab.setRotation(orientation);
  //	else
  //		cab.setFillColor(sf::Color::Yellow);
  //	cab.setOutlineThickness(2);
  //	cab.setOutlineColor(sf::Color::White);
}

void Render::createWall(std::vector<sf::RectangleShape> &wall) {
  WallPosition wp;
  sf::RectangleShape rect;
  sf::Vector2f offset = getOffset();
  int numWalls = env.wall.getNumWalls();
  wall.clear();
  for (int i = 0; i < numWalls; i++) {
    wp = env.wall.getWallPosition(i);
    if (wp.y0 == wp.y1) // horizontal wall
    {
      rect.setOrigin(sf::Vector2f(0, WALL_Y / 2));
      rect.setPosition(sf::Vector2f(offset.x + GRID_SIZE * wp.x0,
                                    offset.y + GRID_SIZE * wp.y0));
      rect.setSize(sf::Vector2f(GRID_SIZE * abs(wp.x0 - wp.x1), WALL_Y));
      rect.setFillColor(sf::Color::White);
      rect.setOutlineThickness(0);
      rect.setOutlineColor(sf::Color::White);
      rect.setTexture(&wallTexture);
      wall.push_back(rect);
    } else if (wp.x0 == wp.x1) // vertical wall
    {
      rect.setOrigin(sf::Vector2f(WALL_X / 2, 0));
      rect.setPosition(sf::Vector2f(offset.x + GRID_SIZE * wp.x0,
                                    offset.y + GRID_SIZE * wp.y0));
      rect.setSize(sf::Vector2f(WALL_X, GRID_SIZE * abs(wp.y0 - wp.y1)));
      rect.setFillColor(sf::Color::White);
      rect.setOutlineThickness(1);
      rect.setOutlineColor(sf::Color::Black);
      rect.setTexture(&wallTexture);
      wall.push_back(rect);
    }
  }
}

void Render::resetTextColor(sf::Text &R, sf::Text &G, sf::Text &B,
                            sf::Text &Y) {
  R.setFillColor(sf::Color::White);
  G.setFillColor(sf::Color::White);
  B.setFillColor(sf::Color::White);
  Y.setFillColor(sf::Color::White);
}

void Render::createPassenger(sf::Text &R, sf::Text &G, sf::Text &B,
                             sf::Text &Y) {

  resetTextColor(R, G, B, Y);
  switch (env.passenger.getCode(0)) {
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
  switch (env.passenger.getCode(1)) {
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

  sf::Vector2f offset = getOffset();
  float relativePosX = (float)(2*env.passenger.pickUpPos.x + 1) / 2;
  float relativePosY = (float)(2*env.passenger.pickUpPos.y + 1) / 2;
  srcBlimp.setOrigin(srcBlimp.getGlobalBounds().width / 2, srcBlimp.getGlobalBounds().height / 2);
  srcBlimp.setPosition(sf::Vector2f(offset.x + relativePosX * GRID_SIZE,
              offset.y + relativePosY * GRID_SIZE));
  srcBlimp.setSize(sf::Vector2f(GRID_SIZE/2, GRID_SIZE/2));

  relativePosX = (float)(2*env.passenger.dropOffPos.x + 1) / 2;
  relativePosY = (float)(2*env.passenger.dropOffPos.y + 1) / 2;
  destBlimp.setOrigin(destBlimp.getGlobalBounds().width / 2, destBlimp.getGlobalBounds().height / 2);
  destBlimp.setPosition(sf::Vector2f(offset.x + relativePosX * GRID_SIZE,
              offset.y + relativePosY * GRID_SIZE));
  destBlimp.setSize(sf::Vector2f(GRID_SIZE/2, GRID_SIZE/2));
}

void Render::drawNDisplay(std::vector<sf::RectangleShape> &pl,
                          sf::RectangleShape &cab,
                          std::vector<sf::RectangleShape> &wall, sf::Text &R,
                          sf::Text &G, sf::Text &B, sf::Text &Y,
                          sf::Text &info) {

  for (int i = 0; i < NUM_GRIDS_X; i++) {
    for (int j = 0; j < NUM_GRIDS_Y; j++) {
      window.draw(pl[IX(i, j)]);
    }
  }

  window.draw(R);
  window.draw(G);
  window.draw(B);
  window.draw(Y);
  window.draw(destBlimp);
  window.draw(info);

  window.draw(cab);
  window.draw(srcBlimp);
  for (int i = 0; i < wall.size(); i++) {
    window.draw(wall[i]);
  }
  window.display();
}

void Render::learn(std::vector<sf::RectangleShape> &pl, sf::RectangleShape &cab,
                   std::vector<sf::RectangleShape> &wall, sf::Text &textR,
                   sf::Text &textG, sf::Text &textB, sf::Text &textY) {
  sf::Text info;
  sf::Font font; // for RGBY markings
  if (!font.loadFromFile("res/arial.ttf")) {
    printf("Error in loading font from file\n");
  }
  int epochs, penalties, reward, score;
  bool done;
  int cabI, cabJ, passengerIdx, destIdx, state, nextState;
  float oldQ, newQ, nextMaxQ;
  int actionCode;
  float epsilon = EPSILON;
  int iter = env.iterator;
  std::string mode;
  float adjustableWaitTime = 0.3;

  for (; iter < NUM_ITERATIONS && window.isOpen(); iter++) {
    env.reset(); // reset our sample space
    updateFigure(cab, wall, textR, textG, textB, textY);
    cabI = env.cab.getSpawnPosition('x');
    cabJ = env.cab.getSpawnPosition('y');

    //		if (env.passenger.getPassengerStatus())
    //			passengerIdx = 4;
    //		else{
    if (env.passenger.getCode(0) == 'R')
      passengerIdx = 0;
    else if (env.passenger.getCode(0) == 'G')
      passengerIdx = 1;
    else if (env.passenger.getCode(0) == 'B')
      passengerIdx = 2;
    else if (env.passenger.getCode(0) == 'Y')
      passengerIdx = 3;
    //		}
    if (env.passenger.getCode(1) == 'R')
      destIdx = 0;
    else if (env.passenger.getCode(1) == 'G')
      destIdx = 1;
    else if (env.passenger.getCode(1) == 'B')
      destIdx = 2;
    else if (env.passenger.getCode(1) == 'Y')
      destIdx = 3;
    state = env.encode(cabI, cabJ, passengerIdx, destIdx);
    epochs = 0;
    penalties = 0;
    reward = 0;
    done = false;
    score = 0;
    while (!done) {
      sf::Event e;
      while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
          window.close();
        if(e.type == sf::Event::MouseButtonPressed){
            if(e.mouseButton.button == sf::Mouse::Left){
                sf::Vector2f offset = getOffset();
                env.wall.buildWall(e.mouseButton.x - (int)(offset.x), e.mouseButton.y - (int)(offset.y));
                env.initializeRewardTable();
                createWall(wall);
            }
            else if(e.mouseButton.button == sf::Mouse::Right){
                sf::Vector2f offset = getOffset();
                env.wall.destroyWall();
                env.initializeRewardTable();
                createWall(wall);
            }
        }
        if (e.type == sf::Event::KeyPressed) {
          if (e.key.code == sf::Keyboard::Up) {
            adjustableWaitTime += 0.01;
            printf("adjustableWaitTime = %f\n", adjustableWaitTime);
          } else if (e.key.code == sf::Keyboard::Down) {
            adjustableWaitTime =
                ((adjustableWaitTime - 0.01 < 0) ? 0
                                                 : (adjustableWaitTime - 0.01));
            printf("adjustableWaitTime = %f\n", adjustableWaitTime);
          }
        }
      }
      window.clear();
      float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

      /* ----- I'm messing about in this section ---- */
      int desiredNumEpisodes = 100; // exploration almost ceases to exist (except after allowedEpochs) after these many episodes.
      float allowedEpochs = 30.0; // after this time, the cab starts exploring more thinking that he's stuck.
      epsilon = iter < desiredNumEpisodes 
          ? EPSILON * (desiredNumEpisodes - iter) / desiredNumEpisodes 
          : 0.01; /* one possibility */
      epsilon = epochs > allowedEpochs 
          ? EPSILON * (epochs / (epochs + allowedEpochs)) 
          : epsilon;
      /* ------- ------- */

      if(epochs)
      if (r < epsilon) {
        mode = "Explore";
        actionCode = std::rand() % NUM_ACTIONS; // exploration
      } else {
        mode = "Exploit";
        actionCode = env.getActionForMaxQValue(state); // exploitation
      }
      env.step(actionCode, state, nextState, reward, done);
      oldQ = env.qTable[E(state, actionCode)];
      nextMaxQ = env.getMaxQForState(nextState);
      newQ = ((1 - ALPHA) * oldQ) + (ALPHA * (reward + GAMMA * nextMaxQ));
      env.qTable[E(state, actionCode)] = newQ;
      if (reward == -10)
        penalties += 1;
      score += reward;
      state = nextState;
      epochs += 1;

      env.decode(state, cabI, cabJ, passengerIdx, destIdx);
      std::string inf =
          "Episode no.      : " + std::to_string((int)(iter + 1)) + "\n" +
          "Action                : " + env.actionCodeToString(actionCode) +
          "\n" + "State                 : " + std::to_string((int)state) +
          "\n" + "Cab Location    : " + "(" + std::to_string((int)cabI) + "," +
          std::to_string((int)cabJ) + ")\n" +
          "Mode                  : " + mode + "\n" +
          "Time spent      : " + std::to_string((int)epochs) + "\n" +
          "score                 : " + std::to_string((int)score);
      setTextProperties(info, font, 16, inf);

      //			printf("%d\n",actionCode);
      stepFigure(nextState, pl, cab, wall, textR, textG, textB, textY);
      drawNDisplay(pl, cab, wall, textR, textG, textB, textY, info);

      usleep(adjustableWaitTime * 1000000);
      //			if(epochs <= 10){
      //				usleep(0.3 * 1000000);
      //			}
      if (epochs > 1000) // explore more if stuck
        epsilon = ((EPSILON + 2 * float(epochs) / 10000) > 0.9
                       ? 0.9
                       : (EPSILON + 2 * float(epochs) / 10000));
    }
    env.iterator = iter;
    env.saveQTableToFile("./saveData/qTable.dat");
    env.saveOtherStats("./saveData/stats.dat", epochs, score);
  }
}

void Render::stepFigure(int nextState, std::vector<sf::RectangleShape> &pl,
                        sf::RectangleShape &cab,
                        std::vector<sf::RectangleShape> &wall, sf::Text &textR,
                        sf::Text &textG, sf::Text &textB, sf::Text &textY) {
  int code = nextState;
  int cabI, cabJ, passengerIdx, destIdx;
  env.decode(code, cabI, cabJ, passengerIdx, destIdx);
  env.cab.setCurrPosition(cabI, cabJ);
  createCab(cab);
  //	createWall(wall);
  //	createPassenger(textR, textG, textB, textY);
}
