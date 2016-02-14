/******************************************************************************
   Filename: ChessPiece.h

     Author: David C. Drake (http://davidcdrake.com)

Description: Header file for the ChessPiece class.
******************************************************************************/

#ifndef CHESS_PIECE_H_
#define CHESS_PIECE_H_

enum ChessPieceType {
  PAWN = 100,
  ROOK,
  BISHOP,
  KNIGHT,
  QUEEN,
  KING,
  NUM_CHESS_PIECE_TYPES
};

enum ChessPieceColor {
  WHITE,
  BLACK,
  NUM_CHESS_PIECE_COLORS
};

class ChessPiece {
 public:
  ChessPiece(int type, int color, int row, int col);
  ~ChessPiece();
  int getType() const { return type_; }
  int getColor() const { return color_; }
  int getRow() const { return row_; }
  int getCol() const { return col_; }
  double getX() const { return x_; }
  double getY() const { return y_; }
  double getZ() const { return z_; }
  void moveTo(int row, int col);
  void die();
 private:
  int type_, color_, row_, col_;
  double x_, y_, z_;
};

#endif  // CHESS_PIECE_H_
