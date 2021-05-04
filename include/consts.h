#pragma once

#define WINDOW_SIZE_X 700
#define WINDOW_SIZE_Y 700
#define GRID_SIZE 80
#define NUM_GRIDS_X 5
#define NUM_GRIDS_Y 5
#define CAB_X 3 * GRID_SIZE/10
#define CAB_Y 7 * GRID_SIZE/10
#define NUM_WALLS 5
#define WALL_X 10 // width of wall if horizontal
#define WALL_Y 10 // width of wall if vertical
#define NUM_ACTIONS 6

#define NUM_PASSENGER_STATES 5
#define NUM_DEST_STATES 4

#define NUM_ITERATIONS 100000
#define NEG_LIM 0 //-100

#define ALPHA 0.1
#define GAMMA 0.6
#define EPSILON 0.1

// wall location hardcoded into src/wall.cpp
