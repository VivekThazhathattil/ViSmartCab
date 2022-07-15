#pragma once

#define WINDOW_SIZE_X 700
#define WINDOW_SIZE_Y 700

#define GRID_SIZE 80

#define NUM_GRIDS_X 5
#define NUM_GRIDS_Y 5

#define CAB_X 3 * GRID_SIZE / 10
#define CAB_Y 7 * GRID_SIZE / 10

#define WALL_X 10 // width of wall if horizontal
#define WALL_Y 10 // width of wall if vertical
                  //
/*
 * The six deterministic actions of the taxicab are:
 * 1. move north
 * 2. move south
 * 3. move east
 * 4. move west
 * 5. drop passenger
 * 6. pick passenger
 */
#define NUM_ACTIONS 6

#define NUM_PASSENGER_STATES 5 

#define NUM_DEST_STATES 4

#define NUM_ITERATIONS 100000
#define NEG_LIM 0 //-100

#define ALPHA 0.1
#define GAMMA 0.6
#define EPSILON 0.5

#define MOVE_DOWN 0
#define MOVE_UP 1
#define MOVE_RIGHT 2
#define MOVE_LEFT 3
#define PICKUP 4
#define DROPOFF 5

#define TOLERANCE 10

// wall location hardcoded into src/wall.cpp
