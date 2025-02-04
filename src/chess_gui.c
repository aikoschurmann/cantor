#include "chess_gui.h"
#include "gui.h"


void initialize_sprites() {
    white_pawn_image = load_sprite("../images/pawn_white.png");
    black_pawn_image = load_sprite("../images/pawn_black.png");
    white_knight_image = load_sprite("../images/knight_white.png");
    black_knight_image = load_sprite("../images/knight_black.png");
    white_bishop_image = load_sprite("../images/bishop_white.png");
    black_bishop_image = load_sprite("../images/bishop_black.png");
    white_rook_image = load_sprite("../images/rook_white.png");
    black_rook_image = load_sprite("../images/rook_black.png");
    white_queen_image = load_sprite("../images/queen_white.png");
    black_queen_image = load_sprite("../images/queen_black.png");
    white_king_image = load_sprite("../images/king_white.png");
    black_king_image = load_sprite("../images/king_black.png");
}



void draw_chess_board() {
    const int square_size = (SCREEN_WIDTH - 2 * PADDING) / 8;

    const int light_square_color[4] = {255, 206, 158, 255};
    const int dark_square_color[4] = {209, 139, 71, 255};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Alternate color based on sum of row and column indices
            if ((i + j) % 2 == 0) {
                set_color(light_square_color[0], light_square_color[1], light_square_color[2], light_square_color[3]);
            } else {
                set_color(dark_square_color[0], dark_square_color[1], dark_square_color[2], dark_square_color[3]);
            }

            draw_rectangle(PADDING + j * square_size, i * square_size, square_size, square_size);
        }
    }
}


void chess_coordinates_to_screen(int *x, int *y) {
    
    *x = PADDING + *x * (SCREEN_WIDTH - 2 * PADDING) / 8;
    
    *y = SCREEN_HEIGHT - (*y + 1) * (SCREEN_WIDTH - 2 * PADDING) / 8;
}

void draw_bitboard(Bitboard bitboard, int r, int g, int b, int a) {
    for (int i = 0; i < 64; i++) {
        if (bitboard & (1ULL << i)) {
            int x = i % 8;
            int y = i / 8;
            chess_coordinates_to_screen(&x, &y);
            set_color(r, g, b, a);
            draw_rectangle(x, y, (SCREEN_WIDTH - 2 * PADDING) / 8, (SCREEN_WIDTH - 2 * PADDING) / 8);
        }
    }
}