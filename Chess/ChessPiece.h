/******************************************************************************
   Filename: ChessPiece.h

     Author: David C. Drake (http://davidcdrake.com)

Description: Header file for the ChessPiece class.
******************************************************************************/

#pragma once

enum ChessPieceType
{
  PAWN = 100,
  ROOK,
  BISHOP,
  KNIGHT,
  QUEEN,
  KING,

  NUM_CHESS_PIECE_TYPES
};

enum ChessPieceColor
{
  WHITE,
  BLACK,

  NUM_CHESS_PIECE_COLORS
};

class ChessPiece
{
public:
  ChessPiece(int type, int color, int row, int col);
  ~ChessPiece();
  int getType() const;
  int getColor() const;
  int getRow() const;
  int getCol() const;
  double getX() const;
  double getY() const;
  double getZ() const;
  void moveTo(int row, int col);
  void die();
private:
  int type_, color_, row_, col_;
  double x_, y_, z_;
};
