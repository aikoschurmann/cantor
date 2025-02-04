#ifndef CHESS_GUI_H
#define CHESS_GUI_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define PADDING 160

#include "gui.h"
#include "chess_bitboard.h"

SDL_Texture *white_pawn_image;
SDL_Texture *black_pawn_image;
SDL_Texture *white_knight_image;
SDL_Texture *black_knight_image;
SDL_Texture *white_bishop_image;
SDL_Texture *black_bishop_image;
SDL_Texture *white_rook_image;
SDL_Texture *black_rook_image;
SDL_Texture *white_queen_image;
SDL_Texture *black_queen_image;
SDL_Texture *white_king_image;
SDL_Texture *black_king_image;

void draw_chess_board();
void draw_bitboard(Bitboard bitboard, int r, int g, int b, int a);
void chess_coordinates_to_screen(int *x, int *y);
void initialize_sprites();

#endif