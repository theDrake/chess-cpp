/*******************************************************************************
   Filename: ChessPiece.cc

     Author: David C. Drake (https://davidcdrake.com)

Description: Method definitions for the ChessPiece class.
*******************************************************************************/

#include "chess_piece.h"

ChessPiece::ChessPiece(int type, int color, int row, int col) {
  type_ = type;
  color_ = color;
  row_ = row;
  col_ = col;
}

ChessPiece::~ChessPiece() {}

void ChessPiece::moveTo(int row, int col) {}

void ChessPiece::die() {}
