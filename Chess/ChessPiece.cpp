/******************************************************************************
   Filename: ChessPiece.cpp

     Author: David C. Drake (http://davidcdrake.com)

Description: Method definitions for the ChessPiece class.
******************************************************************************/

#include "ChessPiece.h"

ChessPiece::ChessPiece(int type, int color, int row, int col)
{
  type_  = type;
  color_ = color;
  row_   = row;
  col_   = col;
}

ChessPiece::~ChessPiece()
{}

int ChessPiece::getType() const
{
  return type_;
}

int ChessPiece::getColor() const
{
  return color_;
}

int ChessPiece::getRow() const
{
  return row_;
}

int ChessPiece::getCol() const
{
  return col_;
}

double ChessPiece::getX() const
{
  return x_;
}

double ChessPiece::getY() const
{
  return y_;
}

double ChessPiece::getZ() const
{
  return z_;
}

void ChessPiece::moveTo(int row, int col)
{}

void ChessPiece::die()
{}
