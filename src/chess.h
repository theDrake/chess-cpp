/*******************************************************************************
   Filename: chess.h

     Author: David C. Drake (https://davidcdrake.com)

Description: Header file for chess.cc
*******************************************************************************/

#ifndef CHESS_H_
#define CHESS_H_

#include <cstdlib>
#include <cstdio>
//#include <cassert>
#include <cmath>
#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>
#include <GL/glut.h>
#include "keys.h"
#include "chess_piece.h"

void text_output(double x, double y, char *string);

// Camera-related constants:
#define CAMERA_SPEED        75
#define INTRO_ZOOM_DURATION 4  // seconds
#define INITIAL_EYE_X       -300000
#define DEFAULT_EYE_X       4500
#define DEFAULT_EYE_Y       8000
#define DEFAULT_EYE_Z       -12000
#define Y_MIN               0
#define Y_MAX               50000
#define DEFAULT_AT_X        4500
#define DEFAULT_AT_Y        0
#define DEFAULT_AT_Z        4000

// Board-related constants:
#define BOARD_TOP               -10
#define BOARD_TOP_FRONT_EDGE    -1000
#define BOARD_TOP_BACK_EDGE     10000
#define BOARD_TOP_RIGHT_EDGE    -1000
#define BOARD_TOP_LEFT_EDGE     10000
#define BOTTOM_MODIFIER         0
#define BOARD_BOTTOM            -100000  // BOARD_TOP - BOTTOM_MODIFIER
#define BOARD_BOTTOM_FRONT_EDGE BOARD_TOP_FRONT_EDGE - BOTTOM_MODIFIER
#define BOARD_BOTTOM_BACK_EDGE  BOARD_TOP_BACK_EDGE + BOTTOM_MODIFIER
#define BOARD_BOTTOM_RIGHT_EDGE BOARD_TOP_RIGHT_EDGE - BOTTOM_MODIFIER
#define BOARD_BOTTOM_LEFT_EDGE  BOARD_TOP_LEFT_EDGE + BOTTOM_MODIFIER
#define BOARD_SQUARE_SIZE       1000
#define BOARD_SQUARE_ORIGIN_X   500
#define BOARD_SQUARE_ORIGIN_Z   500
#define BOARD_SQUARE_BORDER     20
#define BOARD_BORDER            500
#define Y_MODIFIER              5

// Color-related constants:
#define BOARD_MAIN_COLOR   redMaterial
#define BOARD_TOP_COLOR    blackMaterial
#define LIGHT_SQUARE_COLOR whiteMaterial
#define DARK_SQUARE_COLOR  blackMaterial
#define LIGHT_PIECE_COLOR  whiteMaterial
#define DARK_PIECE_COLOR   greenMaterial

// Global material-related variables:
GLfloat redMaterial[]         = { 0.7, 0.1, 0.2, 1.0 };
GLfloat greenMaterial[]       = { 0.1, 0.7, 0.4, 1.0 };
GLfloat brightGreenMaterial[] = { 0.1, 0.9, 0.1, 1.0 };
GLfloat blueMaterial[]        = { 0.1, 0.2, 0.7, 1.0 };
GLfloat whiteMaterial[]       = { 1.0, 1.0, 1.0, 1.0 };
GLfloat blackMaterial[]       = { 0.0, 0.0, 0.0, 0.0 };

// Global screen-related variables:
double screen_x = 900;
double screen_y = 600;

// Global camera-related variables:
double eye[3] = { DEFAULT_EYE_X, DEFAULT_EYE_Y, DEFAULT_EYE_Z };
double at[3]  = { DEFAULT_AT_X, DEFAULT_AT_Y, DEFAULT_AT_Z    };

// Global mouse-related variables:
bool leftMouseDown   = false;
bool rightMouseDown  = false;
bool middleMouseDown = false;

#endif  // CHESS_H_
