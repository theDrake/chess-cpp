/******************************************************************************
   Filename: chess.h

     Author: David C. Drake (http://davidcdrake.com), with initial assistance
             from Dr. Barton Stander (http://cit.dixie.edu/faculty/stander.php)

Description: Header file for chess.cpp
******************************************************************************/

#pragma once

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

// "Prototypes" for some color variables defined in chess.cpp:
extern GLfloat redMaterial[];
extern GLfloat greenMaterial[];
extern GLfloat brightGreenMaterial[];
extern GLfloat blueMaterial[];
extern GLfloat whiteMaterial[];
