#ifndef CHESS_BITBOARD_H
#define CHESS_BITBOARD_H

#include "bitboard.h"

typedef enum Piece {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} Piece;

Bitboard white_pawns;
Bitboard white_knights;
Bitboard white_bishops;
Bitboard white_rooks;
Bitboard white_queens;
Bitboard white_king;

Bitboard black_pawns;
Bitboard black_knights;
Bitboard black_bishops;
Bitboard black_rooks;
Bitboard black_queens;
Bitboard black_king;

Bitboard white_pieces;
Bitboard black_pieces;

Bitboard all_pieces;

Bitboard white_bitboards[6];
Bitboard black_bitboards[6];

void initialize_bitboards();
Piece cycle_bitboard(Piece current_bitboard, int direction);


#endif // CHESS_BITBOARD_H