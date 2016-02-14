/******************************************************************************
   Filename: chess.h

     Author: David C. Drake (http://davidcdrake.com)

Description: Header file for chess.cpp
******************************************************************************/

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
#include "glut.h"
#include "keys.h"
#include "ChessPiece.h"

void text_output(double x, double y, char *string);

// Color variables defined in chess.cpp:
extern GLfloat redMaterial[];
extern GLfloat greenMaterial[];
extern GLfloat brightGreenMaterial[];
extern GLfloat blueMaterial[];
extern GLfloat whiteMaterial[];

#endif  // CHESS_H_
